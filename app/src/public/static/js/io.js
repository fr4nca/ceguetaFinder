const socket = io();

socket.on('msg', msg => {
  const h1 = document.createElement('h1');
  h1.innerText = msg.msg;

  const main = document.getElementById('app');
  main.appendChild(h1);
});
