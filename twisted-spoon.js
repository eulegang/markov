const { Distribution } = require("./build/markov")

const initial_state = new Distribution.Weights([1, 0]);

console.log(initial_state.toString())


