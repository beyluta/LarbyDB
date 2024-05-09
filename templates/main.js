import { Database } from "./database.js";

class Program {
    /** 
     * The database instance
     * @type {Database}
    */
    database;

    /**
     * Creates a new instance of the Program class
     * @constructor
     */
    constructor() {
        this.database = new Database("localhost", 8080);
        this.populate();
    }

    async populate() {
        const data = await this.database.queryAll();
        console.log(data);
    }

    /**
     * Reloads the page
     * @returns {void}
     */
    reload() {
        document.location.reload();
    }
}

const program = new Program();
document.querySelector('#reload-btn').addEventListener('click', program.reload);
