const { Location } = require('../../app/models');

class MapController {
  static async get(req, res, next) {
    const locations = await Location.findAll();
    res.json(locations);
  }

  static async post(req, res, next) {
    const loc = await Location.create(req.query);
    req.io.emit('newLoc', loc);
    res.json(loc);
  }
}

module.exports = MapController;
