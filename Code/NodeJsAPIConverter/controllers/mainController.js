import mqtt from "mqtt"
const clientId = "Yaman's Server";
const connectUrl = "mqtt://mqtt.cetools.org:1884";
import cache from "memory-cache";

const MQTT_USER_NAME = 'NAME_HERE';
const MQTT_PASSWORD = 'PASSWORD_HERE';

export default class mainController {
  constructor() {
  }

 async realMQTT(req, res) {
    try{
      var matchResult;

      await fetch('https://worldcupjson.net/matches/current', {
        method: 'GET',
        headers: {
            'Cache-Control': 'no-cache',}
      })
      .then(res => {
          return res.json();
      })
      .then(data => {
          matchResult = data;
          
      })
      .catch(err => console.log(err));
      
      var keyCount  = Object.keys(matchResult).length

      var index;
      if(keyCount > 0) { //More than one match OR just one match currently
        index = 0;

        var mqqtToSend = {
          "home_score": matchResult[index].home_team.goals, 
          "away_score": matchResult[index].away_team.goals, 
          "home_team_en": matchResult[index].home_team.name, 
          "away_team_en": matchResult[index].away_team.name, 
          "finished": matchResult[index].status === "completed"? true:false, 
          "datetime": matchResult[index].datetime
        };
    
          const client = mqtt.connect(connectUrl, {
            clientId,
            clean: true,
            connectTimeout: 4000,
            username: MQTT_USER_NAME,
            password: MQTT_PASSWORD,
            reconnectPeriod: 1000,
          })
          client.publish('student/CASA0022/whatscup/real/currentMatch', JSON.stringify(mqqtToSend))
          
          
          res.send('Success');

      } else if (keyCount == 0) { //No current match
        res.send('No current match');
      }
      
    } catch (err) {
      console.log(err);
      res.sendStatus(500);
    }
  }

  async fakeMQTT(req, res) {
    
    try{
      const fakeDataArray = [
        `{"home_score":0, 
        "away_score":0, 
        "home_team_en":"England", 
        "away_team_en": "France", 
        "finished": "false", 
        "datetime": "2022-11-21 13:01"
      }`,
      `{"home_score":1, 
        "away_score":0, 
        "home_team_en":"England", 
        "away_team_en": "France", 
        "finished": "false", 
        "datetime": "2022-11-21 13:01"
      }`,
      `{"home_score":2, 
        "away_score":0, 
        "home_team_en":"England", 
        "away_team_en": "France", 
        "finished": "false", 
        "datetime": "2022-11-21 13:02"
      }`,
      `{"home_score":2, 
        "away_score":1, 
        "home_team_en":"England", 
        "away_team_en": "France", 
        "finished": "false", 
        "datetime": "2022-11-21 13:03"
      }`,
      `{"home_score":3, 
        "away_score":1, 
        "home_team_en":"England", 
        "away_team_en": "France", 
        "finished": "true", 
        "datetime": "2022-11-21 13:04"
      }`
    ]

      const client = mqtt.connect(connectUrl, {
        clientId,
        clean: true,
        connectTimeout: 4000,
        username: MQTT_USER_NAME,
        password: MQTT_PASSWORD,
        reconnectPeriod: 1000,
      })

      var index = cache.get('fake-index')
      if(index === null)
      {
        index = 0;
        cache.put('fake-index', '0');
      } 
      client.publish('student/CASA0022/whatscup/fake/currentMatch', fakeDataArray[parseInt(index)])
      
      //moving to the next fake item
      if(index === 4)
      {
        cache.put('fake-index', '0');
      } else { 
        cache.put('fake-index', parseInt(index) + 1);
      }

      res.send("Success");
    } catch (err) {
      console.log(err);
      res.sendStatus(500);
    }
  }
}