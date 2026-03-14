import gdb

intp = gdb.lookup_type("int").pointer()
longp = gdb.lookup_type("long").pointer()


class MarkovTransitionWeightPrinter:
    "Print a markov::transition::weights"

    def __init__(self, val):
        self.__val = val

    def _row(self, base, length):
        total = int(base.cast(longp).dereference())
        columns = " ".join(
            [str((base + 2 + offset).dereference()) for offset in range(length)]
        )

        return f"{total}: {columns}"

    def to_string(self):
        length = int(self.__val["len"])
        base = self.__val["base"]

        blen = length + 2
        blen = blen + (blen & 1)

        return "; ".join([self._row(base + blen * x, length) for x in range(length)])


class MarkovTransitionPrinter:
    "Print a markov::transition"

    def __init__(self, val):
        self.__val = val

    def _row(self, base, length):
        elems = ((base + off).dereference() for off in range(length))

        return " ".join([f"{float(elem):.2%}" for elem in elems])

    def to_string(self):
        length = int(self.__val["len"])
        base = self.__val["base"]

        return "; ".join(
            [self._row(base + (length * to), length) for to in range(length)]
        )


class MarkovDistributionWeightsPrinter:
    def __init__(self, val):
        self.__val = val

    def to_string(self):
        length = int(self.__val["len"])
        base = self.__val["base"]

        total = sum(int((base + off).dereference()) for off in range(length))
        vals = " ".join(str(int((base + off).dereference())) for off in range(length))

        return f"{total}: {vals}"


class MarkovDistributionPrinter:
    def __init__(self, val):
        self.__val = val

    def to_string(self):
        length = int(self.__val["len"])
        base = self.__val["base"]

        return " ".join(
            f"{float((base + off).dereference()):.2%}" for off in range(length)
        )


def markov(val):
    if val.type.name == "markov::transition::weights":
        return MarkovTransitionWeightPrinter(val)
    if val.type.name == "markov::transition":
        return MarkovTransitionPrinter(val)
    if val.type.name == "markov::distribution::weights":
        return MarkovDistributionWeightsPrinter(val)
    if val.type.name == "markov::distribution":
        return MarkovDistributionPrinter(val)
    return None


gdb.current_objfile().pretty_printers.append(markov)
