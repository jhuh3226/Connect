
var https = require('https'); // require https
var fs = require('fs'); // Using the filesystem module

// json object with key
var credentials = {
  key: fs.readFileSync('star_itp_io.key'),
  cert: fs.readFileSync('star_itp_io.pem')
};

// Start Normal Express Code
var express = require('express');
var app = express();

app.get('/', function(req, res) {
	res.send("Hello World!");
});
/////

var httpsServer = https.createServer(credentials, app);

httpsServer.listen(443); // Default HTTPS Port