class MapController {
  static get(req, res, next) {
    setInterval(() => {
      req.io.emit('msg', { msg: 'oi from mapcontroller from io' });
    }, 3000);
    res.json({ msg: 'oi from controller from json' });
  }
}

module.exports = MapController;
