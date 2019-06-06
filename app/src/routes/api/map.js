const express = require('express');
const router = express.Router();
const MapController = require('../../controllers/mapController');

router.get('/', MapController.get);

module.exports = router;
