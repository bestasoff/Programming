from types import FunctionType
from typing import Any, Dict

CO_VARARGS = 4
CO_VARKEYWORDS = 8

ERR_TOO_MANY_POS_ARGS = 'Too many positional arguments'
ERR_TOO_MANY_KW_ARGS = 'Too many keyword arguments'
ERR_MULT_VALUES_FOR_ARG = 'Multiple values for arguments'
ERR_MISSING_POS_ARGS = 'Missing positional arguments'
ERR_MISSING_KWONLY_ARGS = 'Missing keyword-only arguments'
ERR_POSONLY_PASSED_AS_KW = 'Positional-only argument passed as keyword argument'


def bind_args(func: FunctionType, *args: Any, **kwargs: Any) -> Dict[str, Any]:
    """Bind values from `args` and `kwargs` to corresponding arguments of `func`

    :param func: function to be inspected
    :param args: positional arguments to be bound
    :param kwargs: keyword arguments to be bound
    :return: `dict[argument_name] = argument_value` if binding was successful,
             raise TypeError with one of `ERR_*` error descriptions otherwise
    """
    function_code = func.__code__
    pos_args_amount = func.__code__.co_argcount
    named_args_amount = func.__code__.co_kwonlyargcount
    arg_names = func.__code__.co_varnames
    pos_only_args = func.__code__.co_posonlyargcount

    pos_args = list(arg_names[:pos_args_amount])
    named_args = list(arg_names[pos_args_amount:pos_args_amount + named_args_amount])
    args_flag = bool(function_code.co_flags & CO_VARARGS)
    kwargs_flag = bool(function_code.co_flags & CO_VARKEYWORDS)
    kek: bool = False

    if args_flag:
        args_name = arg_names[pos_args_amount + named_args_amount]
    if kwargs_flag:
        kek = True
        kwargs_name = arg_names[pos_args_amount + named_args_amount + args_flag]

    given_args = [*args]
    given_kwargs = {**kwargs}

    pos_defaults: Dict[str, Any] = {}
    if func.__defaults__:
        pos_defaults_names = list(pos_args[-1: -len(func.__defaults__) - 1: -1])
        pos_defaults_names.reverse()
        pos_defaults = dict(zip(pos_defaults_names, func.__defaults__))
    named_defaults: Dict[str, Any] = {}
    if func.__kwdefaults__:
        named_defaults = func.__kwdefaults__

    result_dict: Dict[str, Any] = {}
    delta: int = pos_args_amount - len(pos_defaults)

    flag: bool = False
    if pos_only_args != 0 and len(given_args) == 0:
        flag = True

    if len(given_args) > len(pos_args):
        if not args_flag:
            raise TypeError(ERR_TOO_MANY_POS_ARGS)
        else:
            if len(given_args) < min(pos_only_args, delta) or pos_only_args - len(given_args) > delta:
                raise TypeError(ERR_MISSING_POS_ARGS)
            idx: int = 0
            while idx < pos_only_args:
                if pos_args[idx] in given_kwargs and not kek:
                    raise TypeError(ERR_POSONLY_PASSED_AS_KW)
                idx += 1

            result_dict.update(dict(zip(pos_args, given_args[:len(pos_args)])))
            result_dict.setdefault(args_name, tuple(given_args[len(pos_args):]))
    elif len(given_args) == len(pos_args):
        if len(given_args) < min(pos_only_args, delta):
            for el in named_args:
                if el in given_kwargs:
                    raise TypeError(ERR_POSONLY_PASSED_AS_KW)
            raise TypeError(ERR_MISSING_POS_ARGS)
        idx = 0
        while idx < pos_only_args:
            if pos_args[idx] in given_kwargs and not kek:
                raise TypeError(ERR_POSONLY_PASSED_AS_KW)
            idx += 1

        result_dict.update(dict(zip(pos_args, given_args)))
        if args_flag:
            result_dict.setdefault(args_name, tuple())
    else:
        if args_flag:
            result_dict.setdefault(args_name, tuple())
        if len(given_args) < min(pos_only_args, delta):
            for el in named_args:
                if el in given_kwargs and not kek:
                    raise TypeError(ERR_POSONLY_PASSED_AS_KW)
        idx = 0
        while idx < pos_only_args:
            if pos_args[idx] in given_kwargs and not kek:
                raise TypeError(ERR_POSONLY_PASSED_AS_KW)
            idx += 1

        result_dict.update(dict(zip(pos_args[:len(given_args)], given_args)))
        cur_pointer = len(given_args)

        if flag:
            idx = 0
            eroor_flag: bool = False
            while idx < pos_only_args:
                if pos_args[idx] in pos_defaults:
                    eroor_flag = True
                idx += 1
            if not eroor_flag:
                raise TypeError(ERR_MISSING_POS_ARGS)

        while cur_pointer < len(pos_args):
            try:
                if not kek:
                    value = result_dict[pos_args[cur_pointer]]
                    raise TypeError(ERR_MULT_VALUES_FOR_ARG)
            except KeyError:
                pass
            try:
                value = given_kwargs[pos_args[cur_pointer]]
                result_dict.setdefault(pos_args[cur_pointer], value)
                del given_kwargs[pos_args[cur_pointer]]
            except KeyError:
                try:
                    value = pos_defaults[pos_args[cur_pointer]]
                    result_dict.setdefault(pos_args[cur_pointer], value)
                except KeyError:
                    raise TypeError(ERR_MISSING_POS_ARGS)
            cur_pointer += 1

    for arg in pos_args:
        try:
            if not kek:
                value = given_kwargs[arg]
                raise TypeError(ERR_MULT_VALUES_FOR_ARG)
        except KeyError:
            continue
    if len(given_kwargs) > len(named_args) and not kwargs_flag:
        raise TypeError(ERR_TOO_MANY_KW_ARGS)

    cur_pointer = 0
    while cur_pointer < len(named_args):
        try:
            if not kek:
                value = result_dict[named_args[cur_pointer]]
                raise TypeError(ERR_MULT_VALUES_FOR_ARG)
        except KeyError:
            pass
        try:
            value = given_kwargs[named_args[cur_pointer]]
            result_dict.setdefault(named_args[cur_pointer], value)
            del given_kwargs[named_args[cur_pointer]]
        except KeyError:
            try:
                value = named_defaults[named_args[cur_pointer]]
                result_dict.setdefault(named_args[cur_pointer], value)
            except KeyError:
                raise TypeError(ERR_MISSING_KWONLY_ARGS)
        cur_pointer += 1

    if kwargs_flag:
        result_dict.setdefault(kwargs_name, given_kwargs)

    return result_dict
