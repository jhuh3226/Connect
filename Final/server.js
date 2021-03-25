// express is a node module for building HTTP servers
// import the module express
var express = require('express');
// decode the post data
var bodyParser = require('body-parser');
const { Console } = require('console');
// app is the running server
var app = express();

// specify kind of bodyParser
// extended: Unicorn character
var urlencodedBodyParser = bodyParser.urlencoded({ extended: true });
// use middleware - telling express to use the decoder
app.use(urlencodedBodyParser);

var submittedData = [];
var inputData;

// tell express to look in the public  directory forn ayy  files first
app.use(express.static("public"));

// if user put /, then run function(what to do)
app.get("/", function (req, res) {
    res.send("Hello");
});

app.post('/formdata', function (req, res) {

    var dataToSave = {
        value: req.body.value
    }

    submittedData.push(dataToSave);
    console.log(submittedData);

    var output = "<html><body>"
    output += "<div>" + submittedData.value + "</div>"
    output = "<html><body>"



    // for (var i = 0; i < submittedData.length; i++) {
    //     //document.body.style.backgroundColor;
    //     inputData = "<div>"  + submittedData[i].value+"</div>"
    // }
    // var output = "<html><body>" + inputData + "</body></html>";

    res.send(output);
});



// start listening for port
// port: virutal private server to connect to, express normally is 80
app.listen(80, function () {
    console.log('Example running on server 80')
});

