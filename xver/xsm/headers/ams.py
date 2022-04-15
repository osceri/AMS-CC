from functools import reduce
import regex

SM = "ams"

fbbdn = []
fbbdn += ["before"]
fbbdn += ["after"]
fbbdn += ["timer"]
fbbdn += ["state"]
fbbdn += ["super_timer"]
fbbdn += ["super_state"]
fbbdn += ["int8_t"]
fbbdn += ["uint8_t"]
fbbdn += ["int16_t"]
fbbdn += ["uint16_t"]
fbbdn += ["int32_t"]
fbbdn += ["uint32_t"]
fbbdn += ["int64_t"]
fbbdn += ["uint64_t"]
fbbdn += ["float"]
fbbdn += ["double"]

class StateMachine:
    def __init__(self, _id, name, machine, transitions):
        self.name = name
        self.states = []
        self.id = _id
        self.transitions = transitions

        if machine:
            self.repr = regex.findall("\((\w+)->(\w+)\):\[([^\{\}\]]*)\]\{([^\}]*)\}(\{(?:[^{}]|(?R))*\})?", machine)

            states = dict()
            c_transitions = []
            for source, destination, condition, statements, submachine in self.repr:
                c_transitions += [(f"{name}_{source}", f"{name}_{destination}", condition, statements)]
                states[f"{name}_{source}"] = None
                states[f"{name}_{destination}"] = None

            for source, dest, condition, statements, submachine in self.repr:
                if source == dest:
                    states[f"{name}_{source}"] = submachine

            self.states = []
            _id = 0
            for state, state_machine in states.items():
                transitions = [ (src, dst, con, sta) for src, dst, con, sta in c_transitions if src == state ]
                self.states += [ StateMachine(_id, f"{state}", state_machine, transitions) ]
                _id = _id + 1

    def __repr__(self):
        return self.name

def get_states(state):
    states = []
    for sub_state in state.states:
        states += get_states(sub_state)
    states += [state]
    return states

def get_conditionals(state):
    conditionals_list = []
    for sub_state in state.states:
        conditionals_list += get_conditionals(sub_state)
    for source, dest, conditionals, statement in state.transitions:
        conditionals_list += [conditionals]
    return conditionals_list

def get_statements(state):
    statements_list = []
    for source, dest, condition, statements in state.transitions:
        statements_list += statements.split(";")
    for sub_state in state.states:
        statements_list += get_statements(sub_state)
    return statements_list

functions = []
parameters = []
inputs = []
outputs = []
_codec = []
state_machines = []

def codec(var):
    _var = var
    for token, replacement in _codec:
        _var = regex.sub(f"\\b{token}\\b", replacement, _var)
    return _var

with open(f"databases/{SM}.sm") as sm:
    content = sm.read()
    content = content.replace(" ", "").replace("\n", "").replace("\t", "")

    state_machine_tree = StateMachine(0, f"{SM}", content, [])

    all_conditionals = '{' + "}{".join([condition for condition in get_conditionals(state_machine_tree) ]) + '}'
    all_statements = '{' + "}{".join([statement for statement in get_statements(state_machine_tree) ]) + '}'

    def _rec_datas(arg):
        _variables = []
        _functions = []
        for data_name, data_size, data_type, brackets, inner in regex.findall("(\w*_)(\d*)(v0|i8|i16|i32|i64|u8|u16|u32|u64|f32|f64)(\(([^\)]*)\))?", arg):
            if brackets:
                __variables, __functions = _rec_datas(inner)
                _functions += __functions + [(data_name, data_size, data_type, brackets, inner)]
                _variables += __variables
            else:
                _variables += [(data_name, data_size, data_type)]
        return (_variables, _functions)

    type_dict = { "v0" : "void", "i8" : "int8_t", "u8" : "uint8_t", "i16" : "int16_t", "u16" : "uint16_t", "i32" : "int32_t", "u32" : "uint32_t", "i64" : "int64_t", "u64" : "uint64_t", "f32" : "float", "f64" : "double" }

    def _make_funs(_funs):
        __vars = []
        for data_name, data_size, data_type, brackets, inner in _funs:
            if data_size:
                if data_type:
                    __vars += [(data_name + data_size + data_type, int(data_size), type_dict[data_type])]
                else:
                    __vars += [(data_name + data_size + data_type, int(data_size), "uint8_t")]
            else:
                if data_type:
                    __vars += [(data_name + data_size + data_type, 1,  type_dict[data_type])]
                else:
                    __vars += [(data_name + data_size + data_type, 1, "uint8_t")]
        return __vars

    def _make_vars(_vars):
        __vars = []
        for data_name, data_size, data_type in _vars:
            if data_size:
                if data_type:
                    __vars += [(data_name + data_size + data_type, int(data_size), type_dict[data_type])]
                else:
                    __vars += [(data_name + data_size + data_type, int(data_size), "uint8_t")]
            else:
                if data_type:
                    __vars += [(data_name + data_size + data_type, 1,  type_dict[data_type])]
                else:
                    __vars += [(data_name + data_size + data_type, 1, "uint8_t")]
        return __vars

    _get_variables, _get_functions = _rec_datas(all_conditionals)
    _set_variables, _set_functions = _rec_datas(all_statements)
    _functions = list(set(_get_functions + _set_functions))
    _inputs = set(_get_variables)
    _outputs = set(_set_variables)
    _parameters = _inputs.intersection(_outputs)

    inputs = _make_vars(_inputs.difference(_parameters))
    outputs = _make_vars(_outputs.difference(_parameters))
    parameters = _make_vars(_parameters)
    functions = _make_funs(_functions)
    parameters += [("Ts_f32", 1, "float")]

    _codec += [ ("".join(data), f"{SM}_inputs." + "".join(data)) for data, _, _ in inputs ]
    _codec += [ ("".join(data), f"{SM}_outputs." + "".join(data)) for data, _, _ in outputs ]
    _codec += [ ("".join(data), f"{SM}_parameters." + "".join(data)) for data, _, _ in parameters ]

    state_machines = get_states(state_machine_tree)
