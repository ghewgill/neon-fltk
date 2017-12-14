from __future__ import print_function

import json
import os
import sys

Ctypes = {
    "Boolean": "int",
    "Bytes": "Ne_Bytes",
    "Callback": "const Ne_Cell *",
    "Char": "const std::string",
    "Number": "int",
    "String": "const std::string",
}

Pfunction = {
    "Boolean": lambda **kw: "Ne_PARAM_BOOL({index})".format(**kw),
    "Bytes": lambda **kw: "Ne_PARAM_BYTES({index})".format(**kw),
    "Callback": lambda **kw: "Ne_IN_PARAM({index})".format(**kw),
    "Char": lambda **kw: "Ne_PARAM_STRING({index})".format(**kw),
    "Number": lambda **kw: "Ne_PARAM_INT({index})".format(**kw),
    "String": lambda **kw: "Ne_PARAM_STRING({index})".format(**kw),
}

Pwrap = {
    "Boolean": lambda x: x,
    "Bytes": lambda x: x + ".ptr",
    "Callback": lambda x: x,
    "Char": lambda x: x + "[0]",
    "Number": lambda x: x,
    "String": lambda x: x + ".c_str()",
}

Ntypes = {
    "Boolean": "Boolean",
    "Bytes": "Bytes",
    "Char": "String",
    "Number": "Number",
    "String": "String",
}

Retfunc = {
    "Boolean": lambda x: "Ne_RETURN_BOOL({x})".format(x=x),
    "Char": lambda x: "Ne_RETURN_STRING(std::string(1, {x}).c_str())".format(x=x),
    "Number": lambda x: "Ne_RETURN_INT({x})".format(x=x),
    "String": lambda x: "Ne_RETURN_STRING({x})".format(x=x),
}

for t in ["Align", "Boxtype", "Color", "Font", "Fontsize", "Labeltype", "When"]:
    Ctypes[t] = Ctypes["Number"]
    Pfunction[t] = Pfunction["Number"]
    Pwrap[t] = (lambda t: lambda x: "static_cast<Fl_{t}>({x})".format(t=t, x=x))(t)
    Ntypes[t] = t
    Retfunc[t] = Retfunc["Number"]

classes = json.load(open(sys.argv[1]))
if "NOTE" in classes:
    del classes["NOTE"]

for cname, c in classes.items():
    assert cname not in Ctypes
    Ctypes[cname] = c["native"] + " *"
    assert cname not in Pfunction
    Pfunction[cname] = (lambda native: lambda **kw: "Ne_PARAM_POINTER({native}, {index})".format(native=native, **kw))(c["native"])
    assert cname not in Pwrap
    Pwrap[cname] = lambda x: x
    assert cname not in Ntypes
    Ntypes[cname] = cname
    assert cname not in Retfunc
    Retfunc[cname] = lambda x: "Ne_RETURN_POINTER({x})".format(x=x)

with open(os.path.join(sys.argv[2], "fltk.neon.inc"), "w") as neon_inc:
    for cname, c in classes.items():
        if cname.startswith("TODO"):
            continue
        if "constructor" in c:
            params = ", ".join("{pname}: {tname}".format(pname=pname, tname=Ntypes[tname]) for pname, tname in c["constructor"])
            print("EXPORT DECLARE EXTENSION FUNCTION make{cname}({params}): {cname}".format(cname=cname, params=params), file=neon_inc)
        while True:
            if "methods" in c:
                for mname, m in c["methods"].items():
                    if mname.startswith("TODO"):
                        continue
                    params = "".join(", {pname}: {tname}".format(pname=pname, tname=cname+tname if tname == "Callback" else Ntypes[tname]) for pname, tname in m["params"]) if "params" in m else ""
                    returns = ": " + (cname+m["return"] if m["return"] == "Callback" else Ntypes[m["return"]]) if "return" in m else ""
                    print("DECLARE EXTENSION FUNCTION {cname}_{mname}(self: {cname}{params}){returns}".format(
                        cname=cname,
                        mname=mname,
                        params=params,
                        returns=returns,
                    ), file=neon_inc)
                    print("FUNCTION {cname}.{mname}(self: {cname}{params}){returns} {RETURN}{cname}_{mname}(self{args}) END FUNCTION".format(
                        cname=cname,
                        mname=mname,
                        params=params,
                        returns=returns,
                        RETURN="RETURN " if returns else "",
                        args="".join(", {pname}".format(pname=pname) for pname, _ in m["params"]) if "params" in m else "",
                    ), file=neon_inc)
            if "super" not in c:
                break
            c = classes[c["super"]]

with open(os.path.join(sys.argv[2], "fltk.cpp.inc"), "w") as cpp_inc:
    for cname, c in classes.items():
        if cname.startswith("TODO"):
            continue
        if "constructor" in c:
            print("Ne_FUNC(Ne_make{cname})".format(
                cname=cname,
            ), file=cpp_inc)
            print("{", file=cpp_inc)
            index = 0
            for pname, tname in c["constructor"]:
                print("    {ctype} {pname} = {pfunc};".format(
                    ctype=Ctypes[tname],
                    pname=pname,
                    pfunc=Pfunction[tname](
                        cname=cname,
                        index=index,
                    ),
                ), file=cpp_inc)
                index += 1
            print("    {ctype} *r = new {ctype}({args});".format(
                ctype=c["native"],
                args=", ".join("NULL" if pname == "label" else Pwrap[tname](pname) for pname, tname in c["constructor"]),
            ), file=cpp_inc)
            if "label" in [x[0] for x in c["constructor"]]:
                print("    r->copy_label(label.c_str());", file=cpp_inc)
            print("    Ne_RETURN_POINTER(r);", file=cpp_inc);
            print("}", file=cpp_inc)
            print("", file=cpp_inc)
        while True:
            if "methods" in c:
                for mname, m in c["methods"].items():
                    if mname.startswith("TODO"):
                        continue
                    print("Ne_FUNC(Ne_{cname}_{mname})".format(
                        cname=cname,
                        mname=mname,
                    ), file=cpp_inc)
                    print("{", file=cpp_inc)
                    print("    {native} *self = Ne_PARAM_POINTER({native}, 0);".format(
                        native=c["native"] if "native" in c else mname,
                    ), file=cpp_inc)
                    if "params" in m:
                        index = 1
                        for pname, tname in m["params"]:
                            print("    {ctype} {pname} = {pfunc};".format(
                                ctype=Ctypes[tname],
                                pname=pname,
                                pfunc=Pfunction[tname](
                                    cname=cname,
                                    index=index,
                                ),
                            ), file=cpp_inc)
                            index += 1
                    if mname in ["getCallback"]:
                        # TODO: fetch the callback out of the object
                        print("    self = self; // suppress unused local warning", file=cpp_inc)
                        print("    Ne_RETURN_POINTER(NULL);", file=cpp_inc)
                    elif mname in ["setCallback"]:
                        print("    Ne_Cell *thunk = Ne->cell_alloc();", file=cpp_inc)
                        print("    Ne->cell_copy(thunk, cb);", file=cpp_inc)
                        print("    self->callback(generic_callback, thunk);", file=cpp_inc)
                        print("    return Ne_SUCCESS;", file=cpp_inc)
                    else:
                        if "native_call" in m:
                            funcall = m["native_call"]
                        else:
                            funcall = "self->{native}({args})".format(
                                native=m["native"] if "native" in m else mname,
                                args=", ".join(Pwrap[tname](pname) for pname, tname in m["params"]) if "params" in m else "",
                            )
                        if "return" in m:
                            if m["return"] == "String":
                                print("    const char *r = {funcall};".format(
                                    funcall=funcall,
                                ), file=cpp_inc)
                                print("    {retfunc};".format(
                                    retfunc=Retfunc[m["return"]]("r != NULL ? r : \"\""),
                                ), file=cpp_inc)
                            else:
                                print("    {retfunc};".format(
                                    retfunc=Retfunc[m["return"]](funcall),
                                ), file=cpp_inc)
                        else:
                            print("    {};".format(funcall), file=cpp_inc)
                            print("    return Ne_SUCCESS;", file=cpp_inc)
                    print("}", file=cpp_inc)
                    print("", file=cpp_inc)
            if "super" not in c:
                break
            c = classes[c["super"]]
