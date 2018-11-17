/*
 * This code demonstrates a simple HTTP endpoint that receives an Ubidots token
 * and a temperature value as URL parameters, then uses this data to make an
 * HTTP POST request to Ubidots API.
 *
 * You can build a URL as explained below, and paste it into your web browser to
 * test the parser. Example:
 *
 * https://parse.ubidots.com/{your-parser-URL}/?token={YOUR-TOKEN}&device=sample-parser&temperature=45
 */

// Import the 'request-promise' library so we can make HTTP request from the parser
var request = require('request-promise');

// Main function - runs every time the parser is executed.
// "args" is a dictionary containing both the URL params and the HTTP body (for POST requests).
async function main(args) {

    // Log the payload to the console, for debugging purposes. You may access the parser's logs using
    // the option in the above header.
    console.log("data received:");
    console.log(args);

    // Builds the payload to send

    var payload = {"alert": 1};

	// Send the payload to Ubidots
    var ubidots_token = args.token;
    var response = await ubidots_request(ubidots_token, "thinxtra", payload);

    // Log Ubidots response to the console
    console.log(response);

    // Pass Ubidots' API response as the parser's reponse
    return response;
}

// This function builds an HTTP POST request to Ubidots
async function ubidots_request(token, label, body) {
    var options = {
        method: 'POST',
        url: 'https://industrial.api.ubidots.com/api/v1.6/devices/' + label,
        body: body,
        json: true,
        headers: {
            'Content-Type': 'application/json',
            'X-Auth-Token': token
        }
    };
    return await request.post(options);
}
