package lr3_test

import (
	"os"
	"testing"

	"github.com/stretchr/testify/require"

	"lr3/lr3"
)

func TestArrayBasicOps(t *testing.T) {
	a := lr3.NewArray()

	a.PushBack("1")
	a.PushBack("3")
	err := a.InsertAt(1, "2")
	require.NoError(t, err)

	require.Equal(t, 3, a.Size())

	v, err := a.Get(0)
	require.NoError(t, err)
	require.Equal(t, "1", v)

	v, err = a.PopFront()
	require.NoError(t, err)
	require.Equal(t, "1", v)

	v, err = a.PopBack()
	require.NoError(t, err)
	require.Equal(t, "3", v)
}

func TestArrayOutOfRange(t *testing.T) {
	a := lr3.NewArray()

	_, err := a.Get(0)
	require.Error(t, err)

	err = a.Set(1, "x")
	require.Error(t, err)

	err = a.InsertAt(5, "x")
	require.Error(t, err)
}

func TestArrayTextSerialize(t *testing.T) {
	path := "array_txt_test.txt"
	defer os.Remove(path)

	a := lr3.NewArray()
	a.PushBack("a")
	a.PushBack("b")
	require.NoError(t, a.SaveText(path))

	b := lr3.NewArray()
	require.NoError(t, b.LoadText(path))

	require.Equal(t, 2, b.Size())
	v0, _ := b.Get(0)
	v1, _ := b.Get(1)
	require.Equal(t, "a", v0)
	require.Equal(t, "b", v1)
}

func TestArrayBinarySerialize(t *testing.T) {
	path := "array_bin_test.bin"
	defer os.Remove(path)

	a := lr3.NewArray()
	a.PushBack("x")
	a.PushBack("y")
	require.NoError(t, a.SaveBinary(path))

	b := lr3.NewArray()
	require.NoError(t, b.LoadBinary(path))

	require.Equal(t, 2, b.Size())
	v0, _ := b.Get(0)
	v1, _ := b.Get(1)
	require.Equal(t, "x", v0)
	require.Equal(t, "y", v1)
}

func TestArrayPopOnEmpty(t *testing.T) {
	a := lr3.NewArray()

	_, err := a.PopFront()
	require.Error(t, err)

	_, err = a.PopBack()
	require.Error(t, err)
}

func TestArraySetAndOverwrite(t *testing.T) {
	a := lr3.NewArray()
	a.PushBack("a")
	a.PushBack("b")

	err := a.Set(1, "x")
	require.NoError(t, err)

	v, err := a.Get(1)
	require.NoError(t, err)
	require.Equal(t, "x", v)
}

func TestArrayInsertAtEndAndMixFrontBack(t *testing.T) {
	a := lr3.NewArray()

	// Проверяем pushFront + pushBack вместе
	a.PushFront("2")  // [2]
	a.PushBack("3")   // [2,3]
	a.PushFront("1")  // [1,2,3]

	// Вставка в самый конец (i == Size)
	err := a.InsertAt(a.Size(), "4")
	require.NoError(t, err)

	require.Equal(t, 4, a.Size())

	v0, _ := a.Get(0)
	v1, _ := a.Get(1)
	v2, _ := a.Get(2)
	v3, _ := a.Get(3)

	require.Equal(t, "1", v0)
	require.Equal(t, "2", v1)
	require.Equal(t, "3", v2)
	require.Equal(t, "4", v3)
}

func TestArraySaveLoadErrorPaths(t *testing.T) {
	a := lr3.NewArray()
	a.PushBack("x")

	// Путь, который почти гарантированно нельзя создать/открыть
	err := a.SaveText("/root/array_forbidden.txt")
	require.Error(t, err)

	err = a.SaveBinary("/root/array_forbidden.bin")
	require.Error(t, err)

	// И загрузка из несуществующих / недоступных файлов
	err = a.LoadText("/root/array_forbidden.txt")
	require.Error(t, err)

	err = a.LoadBinary("/root/array_forbidden.bin")
	require.Error(t, err)
}
