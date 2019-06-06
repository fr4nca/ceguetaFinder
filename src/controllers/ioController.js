module.exports = io => {
  io.on("connection", socket => {
    socket.on("msg", msg => console.log(msg));
    socket.emit("oi", { msg: "oi" });
  });
};
