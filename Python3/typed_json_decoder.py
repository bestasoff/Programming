import typing as tp
import json

from decimal import Decimal


def decode_typed_json(json_value: str) -> tp.Any:
    """
    Returns deserialized object from json string.
    Checks __custom_key_type__ in object's keys to choose appropriate type.

    :param json_value: serialized object in json format
    :return: deserialized object
    """
    def type_check(dct: tp.Dict[tp.Any, tp.Any]) -> tp.Any:
        val: tp.Any = dct.get("__custom_key_type__")
        if val is not None:
            dct.pop("__custom_key_type__")
            if val == "int":
                dct = {int(key): val for key, val in dct.items()}
            elif val == "float":
                dct = {float(key): val for key, val in dct.items()}
            elif val == "decimal":
                dct = {Decimal(key): val for key, val in dct.items()}
        return dct

    return json.loads(json_value, object_hook=type_check)
