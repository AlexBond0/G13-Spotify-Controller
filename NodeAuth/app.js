/**
 * This is an example of a basic node.js script that performs
 * the Authorization Code oAuth2 flow to authenticate against
 * the Spotify Accounts.
 *
 * For more information, read
 * https://developer.spotify.com/web-api/authorization-guide/#authorization_code_flow
 */

var express = require('express'); // Express web server framework
var request = require('request'); // "Request" library
var cors = require('cors');
var querystring = require('querystring');
var cookieParser = require('cookie-parser');
var open = require('open');

var fs = require('fs');

var client_id = '3637514e0efd45039de424919821c07c'; // Your client id
var client_secret = '38eada464bca477ca258ba765d9129f8'; // Your secret
var redirect_uri = 'http://localhost:8888/callback'; // Your redirect uri

var encoded_client_data = new Buffer(client_id + ':' + client_secret).toString('base64');


/**
 * Generates a random string containing numbers and letters
 * @param  {number} length The length of the string
 * @return {string} The generated string
 */
var generateRandomString = function(length) {
  var text = '';
  var possible = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';

  for (var i = 0; i < length; i++) {
    text += possible.charAt(Math.floor(Math.random() * possible.length));
  }
  return text;
};

var saveTokenData = (body) => {

  // get current time
  var date = new Date()
  var currentTime = date.getTime();

  var tokenJson;
  tokenJson = {
    "access_token" : body.access_token,
    "refresh_token" : body.refresh_token,
    "encoded_client_data" : encoded_client_data,
    "expires_in" : body.expires_in,
    "currentTime" : currentTime,
  }

  fs.writeFile("../G13SpotifyController/tokens.json", JSON.stringify(tokenJson), function(err) {

    if(err) {
        return console.log(err);
    }

    console.log("The file was saved!");
  });
}




var stateKey = 'spotify_auth_state';

var app = express();

app.use(express.static(__dirname + '/public'))
   .use(cors())
   .use(cookieParser());

app.get('/login', function(req, res) {

  var state = generateRandomString(16);
  res.cookie(stateKey, state);

  // your application requests authorization
  var scope = `
    user-read-private 
    user-read-email 
    user-read-playback-state 
    user-modify-playback-state 
    user-read-currently-playing
    user-library-read
    playlist-read-private
    app-remote-control`;
  res.redirect('https://accounts.spotify.com/authorize?' +
    querystring.stringify({
      response_type: 'code',
      client_id: client_id,
      scope: scope,
      redirect_uri: redirect_uri,
      state: state
    }));
});

app.get('/callback', function(req, res) {

  // your application requests refresh and access tokens
  // after checking the state parameter

  var code = req.query.code || null;
  var state = req.query.state || null;
  var storedState = req.cookies ? req.cookies[stateKey] : null;

  if (state === null || state !== storedState) {
    res.redirect('/#' +
      querystring.stringify({
        error: 'state_mismatch'
      }));
  } else {
    res.clearCookie(stateKey);
    var authOptions = {
      url: 'https://accounts.spotify.com/api/token',
      form: {
        code: code,
        redirect_uri: redirect_uri,
        grant_type: 'authorization_code'
      },
      headers: {
        'Authorization': 'Basic ' + encoded_client_data
      },
      json: true
    };

    request.post(authOptions, function(error, response, body) {
      if (!error && response.statusCode === 200) {

        // retreive tokens from responce body
        var access_token  = body.access_token,
            refresh_token = body.refresh_token,
            expires_in    = body.expires_in;

        // save tokens fpr the G13 application to access
        saveTokenData(body);

        // return user info to show we got the right one
        var options = {
          url: 'https://api.spotify.com/v1/me',
          headers: { 'Authorization': 'Bearer ' + access_token },
          json: true
        };

        console.log(options);

        // use the access token to access the Spotify Web API
        request.get(options, function(error, response, body) {
          console.log(body);
        });

        // we can also pass the token to the browser to make requests from there
        res.redirect('/#' +
          querystring.stringify({
            access_token: access_token,
            refresh_token: refresh_token
          }));

        // with the new tokens aquired, shut down the server
        setTimeout(() => {
          process.exit(); 
        }, 
        1000);


      } else {
        res.redirect('/#' +
          querystring.stringify({
            error: 'invalid_token'
          }));
      }
    });
  }
});

app.get('/refresh_token', function(req, res) {

  // requesting access token from refresh token
  var refresh_token = req.query.refresh_token;
  var authOptions = {
    url: 'https://accounts.spotify.com/api/token',
    headers: { 'Authorization': 'Basic ' + encoded_client_data },
    form: {
      grant_type: 'refresh_token',
      refresh_token: refresh_token
    },
    json: true
  };

  request.post(authOptions, function(error, response, body) {
    if (!error && response.statusCode === 200) {
      var access_token = body.access_token;
      res.send({
        'access_token': access_token
      });
    }
  });
});

console.log('Listening on 8888');
app.listen(8888, () => {

  open('http://localhost:8888');
});

