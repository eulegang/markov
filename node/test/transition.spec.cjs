const { test, describe } = require("node:test")
const assert = require("node:assert/strict")
const { Transition } = require("markov")

const weights = [
  [19, 1],
  [0, 1]
]

describe("Transition", () => {
  test("construction", (t) => {
    t.test("by seed", () => {
      new Transition(new Transition.Weights(weights));
    })

    t.test("invalid weights", { expectFailure: true }, () => {
      new Transition(new Transition.Weights(2));
    })
  })

  test("fetch state", () => {
    const transition = new Transition(new Transition.Weights(weights));

    assertFloatEquals(transition.get(0, 0), 0.95, 0.00001)
    assertFloatEquals(transition.get(0, 1), 0.05, 0.00001)
    assertFloatEquals(transition.get(1, 0), 0.00, 0.00001)
    assertFloatEquals(transition.get(1, 1), 1.00, 0.00001)
  })

  test("max state inspection", () => {
    const transition = new Transition(new Transition.Weights(weights));

    assert.strictEqual(transition.states, 2);
  })


  test("toString", () => {
    const transition = new Transition(new Transition.Weights(weights));
    assert.strictEqual(transition.toString(), "95.00% 5.00%; 0.00% 100.00%");
  })
})

function assertFloatEquals(actual, expected, tolerance, message) {
  const difference = Math.abs(actual - expected);
  if (difference >= tolerance) {
    // If the difference is too large, throw an assertion error
    assert.fail({
      actual: actual,
      expected: expected,
      message: message || `Expected ${actual} to be almost equal to ${expected} (tolerance: ${tolerance})`,
      operator: 'assertFloatEquals',
    });
  }
}
