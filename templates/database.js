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
     * @param {string} key
     * @param {number} table
     * @returns {Promise<any>}
     */
    async queryAll(key, table) {
        return await new Promise((resolve, reject) => {
            fetch(`http://${this.#uri}:${this.#port}?key=${key}&table=${table}`, {
                method: "GET",
                headers: {
                    "Content-Type": "application/json",
                },
            })
                .then(async (response) => {
                    resolve(response.json());
                })
                .catch((error) => {
                    reject(error);
                });
        });
    }

    /**
     * Queries all records in range
     * @param {string} key
     * @param {number} table
     * @param {number} start
     * @param {number} end
     * @returns {Promise<any>}
     */
    async queryByRange(key, table, start, end) {
        return await new Promise((resolve, reject) => {
            fetch(`http://${this.#uri}:${this.#port}?key=${key}&table=${table}&from=${start}&to=${end}`, {
                method: "GET",
                headers: {
                    "Content-Type": "application/json",
                },
            })
                .then(async (response) => {
                    resolve(response.json());
                })
                .catch((error) => {
                    reject(error);
                });
        });
    }

    async insertById(id, table, data) {
        return await new Promise((resolve, reject) => {
            fetch(`http://${this.#uri}:${this.#port}?key=${id}&table=${table}`, {
                method: "POST",
                headers: {
                    "Content-Type": "application/json",
                },
                body: JSON.stringify(data),
            })
                .then(async (response) => {
                    resolve(response.json());
                })
                .catch((error) => {
                    reject(error);
                });
        });
    }

    async deleteById(id, table) {
        return await new Promise((resolve, reject) => {
            fetch(`http://${this.#uri}:${this.#port}?key=${id}&table=${table}`, {
                method: "DELETE",
                headers: {
                    "Content-Type": "application/json",
                },
            })
                .then(async (response) => {
                    resolve(response.json());
                })
                .catch((error) => {
                    reject(error);
                });
        });
    }
}