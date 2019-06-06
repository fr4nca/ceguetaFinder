module.exports = {
  username: process.env.DB_USER || 'root',
  password: process.env.DB_PW || '12345',
  database: 'ceguetafinder',
  host: process.env.DB_HOST || '127.0.0.1',
  dialect: 'mysql'
};
