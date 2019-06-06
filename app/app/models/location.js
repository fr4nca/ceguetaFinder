module.exports = (sequelize, DataTypes) => {
  const Location = sequelize.define('Location', {
    lat: DataTypes.STRING,
    long: DataTypes.STRING
  });

  return Location;
};
