const path = require('path');
const express = require('express');
const http = require('http');
const app = express();
const server = http.Server(app);
const io = require('socket.io')(server);
require('./src/controllers/ioController')(io);
const hbs = require('express-handlebars');

// Configs
app.set('view engine', 'hbs');
app.set('views', path.join(__dirname, 'src', 'views'));
app.engine(
  'hbs',
  hbs({
    extname: 'hbs',
    partialsDir: path.join(__dirname, 'src', 'views', 'partials')
  })
);

// Middlewares
app.use(express.static(path.join(__dirname, 'src', 'public', 'static')));

// Routes
app.use('/api/map', require('./src/routes/api/map'));
app.get('/', (req, res) => {
  res.render('index');
});
app.get('/about', (req, res) => {
  res.render('about');
});

const port = process.env.PORT || 3000;

server.listen(port, () => console.log(`Server running on port ${port}`));
