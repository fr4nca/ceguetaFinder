class MapController {
  static get(req, res, next) {
    res.send("oi from controller");
  }
}

module.exports = MapController;
