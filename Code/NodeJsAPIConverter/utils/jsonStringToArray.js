/**
 * Parse stringified json object to Array
 * @param {String} string - stringified json
 * @returns converted Array
 */
export default function jsonStringToArray(string){
    return JSON.parse("[" + string + "]");
}