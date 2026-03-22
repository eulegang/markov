const { test, describe } = require("node:test")
const assert = require("node:assert/strict")
const { Distribution } = require("markov")

describe("Distribution.Weights", () => {
  test("constrcution", (t) => {
    t.test("should throw when constructed with no arguments", { expectFailure: true }, () => {
      new Distribution.Weights()
    })

    t.test("build a empty distribution with 5 states", () => {
      new Distribution.Weights(5)
    })

    t.test("can build with preloaded weights", () => {
      new Distribution.Weights([19, 1])
    })
  })

  test('fetch state', (t) => {
    t.test("n states", () => {
      const dist = new Distribution.Weights(5);
      assert.strictEqual(dist.get(0), 0);
      assert.strictEqual(dist.get(1), 0);
      assert.strictEqual(dist.get(2), 0);
      assert.strictEqual(dist.get(3), 0);
      assert.strictEqual(dist.get(4), 0);
    })

    t.test("seeded", () => {
      const dist = new Distribution.Weights([19, 1]);
      assert.strictEqual(dist.get(0), 19);
      assert.strictEqual(dist.get(1), 1);
    })

    t.test("out of bounds state", { expectFailure: true }, () => {
      const dist = new Distribution.Weights([19, 1]);
      dist.get(3)
    })

    t.test("total", () => {
      const dist = new Distribution.Weights([19, 1]);
      assert.strictEqual(dist.total(), 20);
    })
  })

  test("insertion", () => {
    const dist = new Distribution.Weights([19, 1]);
    dist.insert(0, 80)
    assert.strictEqual(dist.total(), 100);
  })

  test("extension", (t) => {
    t.test("external distribution", () => {
      const dist = new Distribution.Weights([19, 1]);
      const diff = new Distribution.Weights([6, 4]);

      dist.extend(diff);

      assert.strictEqual(dist.total(), 30)
      assert.strictEqual(dist.get(0), 25)
      assert.strictEqual(dist.get(1), 5)
    })

    t.test("internal distribution", () => {
      const dist = new Distribution.Weights([19, 1]);

      dist.extend(dist);

      assert.strictEqual(dist.total(), 40)
      assert.strictEqual(dist.get(0), 38)
      assert.strictEqual(dist.get(1), 2)
    })

    t.test("internal distribution", () => {
      const dist = new Distribution.Weights([19, 1]);

      dist.extend(dist);

      assert.strictEqual(dist.total(), 40)
      assert.strictEqual(dist.get(0), 38)
      assert.strictEqual(dist.get(1), 2)
    })

    t.test("incompatible states", { expectFailure: true }, () => {
      const dist = new Distribution.Weights([19, 1]);
      const diff = new Distribution.Weights([19, 1, 2]);

      dist.extend(diff);
    })
  })

  test("max state inspection", () => {
    const dist = new Distribution.Weights([19, 1]);
    assert.strictEqual(dist.states, 2);
  })

  test("cloning", () => {
    const dist = new Distribution.Weights([19, 1]);
    const other = new Distribution.Weights(dist);

    dist.insert(0, 80);


    assert.strictEqual(dist.total(), 100);
    assert.strictEqual(dist.get(0), 99)
    assert.strictEqual(dist.get(1), 1)

    assert.strictEqual(other.total(), 20);
    assert.strictEqual(other.get(0), 19)
    assert.strictEqual(other.get(1), 1)

  })

  test("toString", (t) => {
    t.test("empty", () => {
      const dist = new Distribution.Weights(3);
      assert.strictEqual(dist.toString(), "0 0 0")
    })

    t.test("seeded", () => {
      const dist = new Distribution.Weights([1, 2, 3]);
      assert.strictEqual(dist.toString(), "1 2 3")
    })
  })
})
