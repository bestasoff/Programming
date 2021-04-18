package main

import (
	"fmt"
	"play.ground/foo"
	"reflect"
	"unsafe"
)

func GetUnexportedField(field reflect.Value) interface{} {
	return reflect.NewAt(field.Type(), unsafe.Pointer(field.UnsafeAddr())).Elem().Interface()
}

func SetUnexportedField(field reflect.Value, value interface{}) {
	reflect.NewAt(field.Type(), unsafe.Pointer(field.UnsafeAddr())).Elem().Set(reflect.ValueOf(value))
}

func main() {
	f := &foo.Foo{
		Exported: "x",
	}

	fmt.Println(f.Exported) // prints "x"
	// fmt.Println(f.unexported) // error: f.unexported undefined (cannot refer to unexported field or method unexported)

	field := reflect.ValueOf(f).Elem().FieldByName("unexported")
	SetUnexportedField(field, "y")
	fmt.Println(GetUnexportedField(field)) // prints "y"
}

-- go.mod --
module play.ground

-- foo/foo.go --
package foo

type Foo struct {
	Exported   string
	unexported string
}
