const socket = io();

socket.emit("msg", { msg: "oi" });
socket.on("oi", msg => console.log(msg));
