import gdb

intp = gdb.lookup_type("int").pointer()
longp = gdb.lookup_type("long").pointer()

class MarkovTransitionWeightPrinter:
    "Print a markov::transition::weights"

    def __init__(self, val):
        self.__val = val

    def to_string(self):
        return "markov transition"

def markov_printer(val):
    if val.type.name == "markov::transition::weights":
        return MarkovTransitionWeightPrinter(val)
    return None

gdb.pretty_printers.append(markov_printer)
