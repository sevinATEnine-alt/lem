const http = require("http");
const fs = require("fs");

var port = 3000;
var host = "0.0.0.0";

var server = http.createServer(function(req, res) {
    var urlSplit = req.url.split("/");
    if (urlSplit[0] == "")
        urlSplit = urlSplit.slice(1)
    
    res.write("eadasd");
});

server.listen(port, host, function() {
    console.log(`Listening on http://${host}:${port}`);
});