import express from "express";
import mainController from "../controllers/mainController.js";

const router = express.Router();

const mainCntler = new mainController();

router.get('/whatscup', (req, res) => {
  mainCntler.realMQTT(req, res);
});

router.get('/fake-whatscup', (req, res) => {
  mainCntler.fakeMQTT(req, res);
});
export default router;
