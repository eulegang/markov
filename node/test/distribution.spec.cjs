const { test, describe } = require("node:test")
const assert = require("node:assert/strict")
const { Distribution } = require("markov")

describe("Distribution", () => {
  test("construction", (t) => {
    t.test("from weights", () => {
      new Distribution(new Distribution.Weights([19, 1]));
    })

    t.test("invalid weights", { expectFailure: true }, () => {
      new Distribution(new Distribution.Weights(2));
    })
  })

  test("fetch state", () => {
    const dist = new Distribution(new Distribution.Weights([19, 1]));

    assertFloatEquals(dist.get(0), 0.95, 0.00001)
    assertFloatEquals(dist.get(1), 0.05, 0.00001)
  })

  test("toString", () => {
    const dist = new Distribution(new Distribution.Weights([19, 1]));

    assert.strictEqual(dist.toString(), "95.00% 5.00%")
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
