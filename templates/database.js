export class Database {
    /**
     * URI of the database
     * @type {string}
     */
    #uri;

    /**
     * Port of the database
     * @type {number}
     */
    #port;

    /**
     * @constructor
     * @param {string} uri 
     * @param {number} port 
     */
    constructor(uri, port) {
        this.#uri = uri;
        this.#port = port;
    }

    /**
     * Queries all records from the database
     * @returns {Promise<any>}
     */
    async queryAll() {
        return await new Promise((resolve, reject) => {
            fetch(`http://${this.#uri}:${this.#port}?table=1&key=ALL`, {
                method: "GET",
                headers: {
                    "Content-Type": "application/json",
                },
            })
                .then((response) => {
                    resolve(response.json());
                })
                .catch((error) => {
                    reject(error);
                });
        });
    }
}