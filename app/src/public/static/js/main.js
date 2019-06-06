const onLoad = async () => {
  const res = await fetch('/api/map');
  const data = await res.json();
  const h1 = document.createElement('h1');
  h1.innerText = data.msg;

  const main = document.getElementById('app');
  main.appendChild(h1);
};

document.addEventListener('DOMContentLoaded', () => {
  onLoad();
});
