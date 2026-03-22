const { test, describe } = require("node:test")
const assert = require("node:assert/strict")
const { Transition } = require("markov")


describe("Transition.Weights", () => {
  test("construction", (t) => {
    t.test("should throw with no arguments", { expectFailure: true }, () => {
      new Transition.Weights()
    })


    t.test("build a empty weights", () => {
      new Transition.Weights(2);
    })

    t.test("build preseeded weights", () => {
      new Transition.Weights([
        [19, 1],
        [0, 1],
      ])
    })
  })

  test("fetch state", (t) => {
    t.test("n states", () => {
      const weights = new Transition.Weights(2);
      assert.strictEqual(weights.get(0, 0), 0);
      assert.strictEqual(weights.get(0, 1), 0);
      assert.strictEqual(weights.get(1, 0), 0);
      assert.strictEqual(weights.get(1, 1), 0);
    })

    t.test("seeded", () => {
      const weights = new Transition.Weights([
        [19, 1],
        [0, 1],
      ]);
      assert.strictEqual(weights.get(0, 0), 19);
      assert.strictEqual(weights.get(0, 1), 1);
      assert.strictEqual(weights.get(1, 0), 0);
      assert.strictEqual(weights.get(1, 1), 1);
    })

    t.test("out of bounds state", { expectFailure: true }, () => {
      const weights = new Transition.Weights([
        [19, 1],
        [0, 1],
      ]);
      weights.get(3, 3)
    })

    t.test("total", () => {
      const weights = new Transition.Weights([
        [19, 1],
        [0, 1],
      ]);
      assert.strictEqual(weights.total(), 21);
    })
  })

  test("insertion", () => {
    const weights = new Transition.Weights([
      [19, 1],
      [0, 1],
    ]);

    weights.insert(0, 0, 80)
    assert.strictEqual(weights.total(), 101);
  })

  test("extension", (t) => {
    t.test("external weight", () => {
      const weights = new Transition.Weights([
        [19, 1],
        [0, 1],
      ]);

      const diff = new Transition.Weights([
        [80, 1],
        [0, 9],
      ]);

      weights.extend(diff);

      assert.strictEqual(weights.total(), 111);
      assert.strictEqual(weights.get(0, 0), 99);
      assert.strictEqual(weights.get(0, 1), 2);
      assert.strictEqual(weights.get(1, 0), 0);
      assert.strictEqual(weights.get(1, 1), 10);
    })

    t.test("internal weight", () => {
      const weights = new Transition.Weights([
        [19, 1],
        [0, 1],
      ]);

      weights.extend(weights);

      assert.strictEqual(weights.total(), 42);
      assert.strictEqual(weights.get(0, 0), 38);
      assert.strictEqual(weights.get(0, 1), 2);
      assert.strictEqual(weights.get(1, 0), 0);
      assert.strictEqual(weights.get(1, 1), 2);
    })

    t.test("incompatible states", { expectFailure: true }, () => {
      const weights = new Transition.Weights([
        [19, 1],
        [0, 1],
      ]);
      const other = new Transition.Weights([
        [19, 1, 3],
        [0, 1, 2],
        [0, 1, 2],
      ]);

      weights.extend(other);
    })
  })

  test("max state inspection", () => {
    const weights = new Transition.Weights([
      [19, 1],
      [0, 1],
    ]);

    assert.strictEqual(weights.states, 2);
  })

  test("cloning", () => {
    const weights = new Transition.Weights([
      [19, 1],
      [0, 1],
    ]);

    const other = new Transition.Weights(weights);

    weights.insert(0, 0, 80);

    assert.strictEqual(weights.total(), 101);
    assert.strictEqual(weights.get(0, 0), 99);
    assert.strictEqual(weights.get(0, 1), 1);
    assert.strictEqual(weights.get(1, 0), 0);
    assert.strictEqual(weights.get(1, 1), 1);

    assert.strictEqual(other.total(), 21);
    assert.strictEqual(other.get(0, 0), 19);
    assert.strictEqual(other.get(0, 1), 1);
    assert.strictEqual(other.get(1, 0), 0);
    assert.strictEqual(other.get(1, 1), 1);

  })
  test("toString", (t) => {
    t.test("empty", () => {
      const weights = new Transition.Weights(3);
      assert.strictEqual(weights.toString(), "0 0 0; 0 0 0; 0 0 0")
    })

    t.test("seeded", () => {
      const weights = new Transition.Weights([
        [19, 1],
        [0, 1],
      ]);
      assert.strictEqual(weights.toString(), "19 1; 0 1")
    })
  })
})

