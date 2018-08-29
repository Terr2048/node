var express = require("express");
var bodyParser = require("body-parser");
var nodeAddon = require("./nodeAddon/build/Release/nodeAddon");
 
var app = express();
var urlencodedParser = bodyParser.urlencoded({extended: false});
 
app.use(express.static(__dirname + "/public"));
 
app.post("/checkGroup", urlencodedParser, function (request, response) {
	if (!request.body) return response.sendStatus(400);
	//console.log(request.body);
	if (nodeAddon.checkGroup(request.body.groupName))
		response.redirect('/groupExists.html');
	else
		response.redirect('/groupDoesNotExist.html');
});
 
app.listen(80);