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
  page = -4;

  /**
   * The number of items per page
   * @type {number}
   */
  items = 4;

  /**
   * The previous items
   * @type {Array<any>}
   */
  pastItems = [];

  /**
   * Creates a new instance of the Program class
   * @constructor
   */
  constructor() {
    this.database = new Database("localhost", 8080);
    this.populateNext = this.populateNext.bind(this);
    this.populatePrevious = this.populatePrevious.bind(this);
    this.save = this.save.bind(this);
    this.populateNext();
  }

  async save(id, data) {
    await this.database.deleteById(id, 1);
    await this.database.insertById(id, 1, data);
  }

  /**
   * Sets up the modals for saving data
   * @param {any} record
   * @returns {HTMLElement}
   */
  setupModals(record) {
    const el = document.createElement("tr");
    const id = document.createElement("td");
    const value = document.createElement("td");
    const total = document.createElement("td");

    el.setAttribute("data-target", "modal");
    el.classList.add("modal-trigger");
    id.innerText = JSON.stringify(record.index | "");
    value.innerText = JSON.stringify(record.value, null, "\t");
    total.innerText = JSON.stringify(record.total);
    el.appendChild(id);
    el.appendChild(value);
    el.appendChild(total);
    document.querySelector("#table").appendChild(el);
    return el;
  }

  /**
   * Populates the page with next data
   * @returns {Promise<void>}
   */
  async populateNext() {
    try {
      this.pastItems?.map((record) => record.remove());
      this.page += this.items;
      const data = await this.database.queryByRange(
        "ALL",
        1,
        this.page,
        this.page + this.items
      );
      this.total = Number(data[0].total);
      this.pastItems = data.map(this.setupModals);
    } catch (error) {
      console.error(error);
    }
  }

  /**
   * Populates the page with previous data
   * @returns {Promise<void>}
   */
  async populatePrevious() {
    try {
      this.pastItems?.map((record) => record.remove());
      this.page -= this.items;
      const data = await this.database.queryByRange(
        "ALL",
        1,
        this.page,
        this.page + this.items
      );
      this.total = Number(data[0].total);
      this.pastItems = data.map(this.setupModals);
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
document.querySelector("#reload-btn").addEventListener("click", program.reload);
document
  .querySelector("#next-btn")
  .addEventListener("click", program.populateNext);
document
  .querySelector("#prev-btn")
  .addEventListener("click", program.populatePrevious);
