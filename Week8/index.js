var express = require("express");
const app = express();


// make all the files in 'public' available, make frontend website visible
app.use("/", express.static("public"));

function getReadings(request, response){
    let clientAddress = request.connection.remoteAddress;
    console.log(clientAddress);
    response.end("Hi there" + clientAddress);
}

app.listen(process.env.PORT | 8080);
app.get("/readings", getReadings);
//app.post("/data", saveData);