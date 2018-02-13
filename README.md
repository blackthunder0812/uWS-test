gen key:
openssl req -x509 -newkey rsa:2048 -keyout pkey.pem -out cert.pem -days 3650

client setup:
var exampleWebSocket = new WebSocket("wss://localhost.localdomain:3000/");
exampleWebSocket.onmessage = function (event) {
    console.log(event);
};
exampleWebSocket.onclose = function (event) {
    console.log(event);
};

client usage:
exampleWebSocket.send("[Protobuf serialized service message]")
