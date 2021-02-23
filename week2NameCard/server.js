// import node package and express is one them
var express = require('express')
// decode the post data
var bodyParser = require('body-parser');
const { Console } = require('console');
var app = express();

// specify kind of bodyParser
// extended: Unicorn character
var urlencodedBodyParser = bodyParser.urlencoded({ extended: true });
// use middleware - telling express to use the decoder
app.use(urlencodedBodyParser);

var submittedData = [];
var inputData;

// anything in the public directory it will run - middle ware
app.use(express.static('public'));

// do this when user puts /
// default route
app.get('/', function (req, res) {
    res.send('Hello World');
});

// route receiving the data when somebody submit the form
app.post('/formdata', function (req, res) {
    //console.log(req.body.data);
    //console.log(req.query.data);

    /* storing data
    var dataToSave = new Object();
    dataToSave = req.body.data
    */

    // JSON method
    var dataToSave = {
        name: req.body.name,
        phone: req.body.phone,
        email: req.body.email,
        color: req.body.color
    };

    // storing input data and push
    submittedData.push(dataToSave);
    console.log(submittedData);

    // output the result
    // var output = "<html><body>"
    var title = "<h3 style='padding: 14px 16px'> Your Name Card </h3>";
    for (var i = 0; i < submittedData.length; i++) {
        //document.body.style.backgroundColor;
        inputData = "<div style = 'color: "+ submittedData[i].color+"; padding: 14px 16px'>" + submittedData[i].name + "<br>" + submittedData[i].phone + "<br>" + submittedData[i].email + "</div>"
    }
    var output = "<html><body>"+ title + inputData + "</body></html>";

    res.send(output);

    //send back
    //res.send("Got your data! You submitted: " + req.body.name + "phone" + req.body.phone + req.body.color);
});

// 30 developement, 80 is default
app.listen(8080, function () {
    console.log('Example app listening on port 8080!');
});
