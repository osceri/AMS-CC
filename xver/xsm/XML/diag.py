import regex

xml = "graph"

output = ""

with open("XML/" + xml + ".drawio") as diag:
    content = diag.read()
    xml_weird = [('\n', ''), ('\t', ''), ('&quot;', '"'), ('&amp;amp;', '&'), ('&apos;', "'"), ('&lt;', '<'), ('&gt;', '>'), ('&amp;', '&'), ('<div>', ''), ('</div>', ''), ('<br>', '')]
    for x, y in xml_weird:
        content = content.replace(x, y)
    for x, y in xml_weird:
        content = content.replace(x, y)

    first = True

    transitions = []
    transitions_dict = dict()
    
    for id, page in regex.findall('<diagram id="([^"]*)"[^>]*>(.*?)<\/diagram>', content):
        transitions_dict[id] = []

        ist = regex.findall('id="([^"]*)" style(.*?)source="([^"]*)"(.*?)target="([^"]*)"', page)
        ivp = regex.findall('id="([^"]*)" value="([^"]*)"(.*?)parent="([^"]*)"', page)
        nli = regex.findall('(UserObject|object) label="([^"]*)".*?(link="data:page\/id,([^"]*)")? id="([^"]*)"', page)

        _states = dict()
        _lines = dict()
        _links = dict()
        _transitions = dict()

        for _, v, _, p in ivp:
            _lines[p] = v

        for _, n, _, l, i in nli:
            _states[i] = n
            if l:
                _links[i] = l

        empty_subs = [ (i, None, i, None, i) for i in _links if not any((s==t)and(s==i) for _, _, s, _, t in ist) ]
        
        for i, _, s, _, t in ist + empty_subs:
            _s = _states[s]
            _t = _states[t]

            if _lines.get(i):
                for C, S in regex.findall('(\[[^]]*\])?(\{[^}]*\})', _lines[i]):
                    _C = C
                    _S = S
            else:
                _C = "[]"
                _S = "{}"

            if _links.get(i):
                _L = _links[i]
            else:
                _L = None

            if first:
                transitions += [(_s, _t, _C, _S, _L)]
            else:
                transitions_dict[id] += [(_s, _t, _C, _S, _L)]
        first = False

    def plog(r, x):
        global output
        output += "    "*r + x + '\n'
    
    def rec_add(r, T):
        for s, t, C, S, L in T:
            if transitions_dict.get(L):
                plog(r, f"({s} -> {t}) : {C} {S}" + " {")
                rec_add(r + 1, transitions_dict[L])
                plog(r, "}")
            else:
                plog(r, f"({s} -> {t}) : {C} {S}" + " {}")

    rec_add(0, transitions)

with open("XML/" + xml + ".sm", "w") as sm:
    sm.write(output)
    print(output)




