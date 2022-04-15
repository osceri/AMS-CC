import regex


with open("graph.drawio") as diag:
    content = diag.read()

    transition_data = dict()

    parents = dict()

    ipst = regex.findall('id="([^"]*)".*parent="([^"]*)" source="([^"]*)" target="([^"]*)"', content)

    for id, parent, source, target in ipst:
        # lines
        transition_data[id] = dict()
        transition_data[id]["source"] = source
        transition_data[id]["target"] = target
        transition_data[id]["transition"] = id
        transition_data[id]["parent"] = parent
        parents[parent] = None

    for _parent in parents:
        for _, parent, source, target in ipst:
            if (_parent == source) or (_parent == target):
                transition_data["__" + _parent] = dict()
                transition_data["__" + _parent]["source"] = _parent
                transition_data["__" + _parent]["target"] = _parent
                transition_data["__" + _parent]["transition"] = -1
                transition_data["__" + _parent]["parent"] = parent
                break

    name_codec = dict()
    line_codec = dict()
    own_codec = dict()

    cvp = regex.findall('id="([^"]*)" value="([^"]*)".*parent="([^"]*)"', content)

    for child, value, parent in cvp:
        # is either the transition data of a line, or it is a state nesetd in another state
        if regex.findall('(\[[^]]*\]) ?(\{[^}]*\})', value):
            line_codec[parent] = value
        else:
            own_codec[child] = parent
            name_codec[child] = value

    name_codec['1'] = '1'

    _transition_data = dict()
    for key, data in transition_data.items():
        _transition_data[key] = dict()
        source = name_codec[data["source"]]
        _transition_data[key]["source"] = source
        target = name_codec[data["target"]]
        _transition_data[key]["target"] = target

        if line_codec.get(data["transition"]):
            line = line_codec[data["transition"]]
        else:
            line = "[]{}"
        _transition_data[key]["transition"] = line

        parent = name_codec[data["parent"]]
        _transition_data[key]["parent"] = parent

    def p_t(r, x):
        print("    "*r + x)

    def add_text(r, p):
        for _, data in _transition_data.items():
            source = data["source"]
            target = data["target"]
            transition = data["transition"]
            parent = data["parent"]

            if parent == p:
                if source != target:
                    p_t(r, f"({source} -> {target}) : {transition}" + " {")
                    p_t(r, "}")
                else:
                    p_t(r, f"({source} -> {target}) : {transition}" + " {")
                    add_text(r + 1, source)
                    p_t(r, "}")
                p_t(0, "")

    add_text(0, '1')


