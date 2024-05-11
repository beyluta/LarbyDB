import { Database } from "./database.js";

class Program {
    /** 
     * The database instance
     * @type {Database}
    */
    database;

    /**
     * The total number of records
     * @type {number}
    */
    total = 0;

    /**
     * The current page
     * @type {number}
    */
    page = 0;


    /**
     * Creates a new instance of the Program class
     * @constructor
     */
    constructor() {
        this.database = new Database("localhost", 8080);
        this.populateNext();
    }

    /**
     * Populates the page with data
     * @returns {Promise<void>}
     */
    async populateNext() {
        try {
            const data = await this.database.queryByRange('ALL', 1, this.page, this.page + 4);
            this.total = data[0].total;
            this.page += 4;

            for (const record of data) {
                const el = document.createElement('a');
                el.setAttribute('class', 'collection-item');
                el.innerText = JSON.stringify(record.value, null, '\t');
                document.querySelector('.collection').appendChild(el);
            }

            console.log(data, this.total);
        } catch (error) {
            console.error(error);
        }
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
