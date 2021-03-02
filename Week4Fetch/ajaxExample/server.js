// import nedb to store data
var dataStore = require('nedb');
var db = new dataStore({ filename: "data.json", autoload: true });

// import node package and express is one them
var express = require('express');
const { Console } = require('console');
var app = express();

// anything in the public directory it will run - middle ware
app.use(express.static('public'));

// route receiving the data when somebody submit the form
app.get('/formdata', function (req, res) {

    console.log(req.query.data);

    // JSON method
    var dataToSave = {
        time: req.query.time,
        text: req.query.paragraph_text,
        title: req.query.title,
        color: req.query.color
    };

    // Insert the data into the database
    db.insert(dataToSave, function (err, newDocs) {
        // define everything in the database
        // wrap it up in JSON format
        // put that into the template
        db.find({}, function (err, docs) {
            // var dataWrapper = { data: docs };
            // res.render("outputTemplate.ejs", dataWrapper)

            // get all the data in this js
            res.send(docs);
        });
    });


});

// 30 developement, 80 is default
app.listen(80, function () {
    console.log('Example app listening on port 80!');
});
