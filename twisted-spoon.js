const { Distribution, Transition } = require("./build/markov")

const initial_state = new Distribution.Weights([1, 0]);
const state = new Distribution(initial_state);

const chance_weight = new Transition.Weights([
  [19, 1],
  [0, 1],
]);

const chances = new Transition(chance_weight);

console.log(state.toString());
console.log(chances.toString());

let next = chances.apply(state);
for (let i = 0; i < 100; i++) {
  console.log(`${i + 1}: ${next}`);

  next = chances.apply(next);
}
