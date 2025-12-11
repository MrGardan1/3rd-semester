package lr3_test

import (
	"os"
	"testing"

	"github.com/stretchr/testify/require"

	"lr3/lr3"
)

func TestStackBasic(t *testing.T) {
	s := lr3.NewStack()

	s.Push("a")
	s.Push("b")
	s.Push("c")

	require.Equal(t, 3, s.Size())

	v, err := s.Peek()
	require.NoError(t, err)
	require.Equal(t, "c", v)

	v, err = s.Pop()
	require.NoError(t, err)
	require.Equal(t, "c", v)

	v, err = s.Pop()
	require.NoError(t, err)
	require.Equal(t, "b", v)

	v, err = s.Pop()
	require.NoError(t, err)
	require.Equal(t, "a", v)

	_, err = s.Pop()
	require.Error(t, err)
}

func TestStackEmptyPeek(t *testing.T) {
	s := lr3.NewStack()
	_, err := s.Peek()
	require.Error(t, err)
}

func TestStackTextSerialize(t *testing.T) {
	path := "stack_txt_test.txt"
	defer os.Remove(path)

	s := lr3.NewStack()
	s.Push("one")
	s.Push("two")
	require.NoError(t, s.SaveText(path))

	s2 := lr3.NewStack()
	require.NoError(t, s2.LoadText(path))

	require.Equal(t, 2, s2.Size())
	v, _ := s2.Pop()
	require.Equal(t, "two", v)
	v, _ = s2.Pop()
	require.Equal(t, "one", v)
}

func TestStackBinarySerialize(t *testing.T) {
	path := "stack_bin_test.bin"
	defer os.Remove(path)

	s := lr3.NewStack()
	s.Push("x")
	s.Push("y")
	require.NoError(t, s.SaveBinary(path))

	s2 := lr3.NewStack()
	require.NoError(t, s2.LoadBinary(path))

	require.Equal(t, 2, s2.Size())
	v, _ := s2.Pop()
	require.Equal(t, "y", v)
	v, _ = s2.Pop()
	require.Equal(t, "x", v)
}

func TestStackSerializeEmpty(t *testing.T) {
	txt := "stack_empty_txt.txt"
	bin := "stack_empty_bin.bin"
	defer os.Remove(txt)
	defer os.Remove(bin)

	s := lr3.NewStack()

	// сохранение пустого стека не должно падать
	require.NoError(t, s.SaveText(txt))
	require.NoError(t, s.SaveBinary(bin))

	s2 := lr3.NewStack()
	require.NoError(t, s2.LoadText(txt))
	require.Equal(t, 0, s2.Size())

	s3 := lr3.NewStack()
	require.NoError(t, s3.LoadBinary(bin))
	require.Equal(t, 0, s3.Size())
}

func TestStackLoadNonExistingFiles(t *testing.T) {
	s := lr3.NewStack()

	// попытка загрузки из несуществующих файлов должна вернуть ошибку, но не паниковать
	err := s.LoadText("no_such_stack_txt.txt")
	require.Error(t, err)

	err = s.LoadBinary("no_such_stack_bin.bin")
	require.Error(t, err)
}

func TestStackSaveLoadChain(t *testing.T) {
	txt := "stack_chain_txt.txt"
	bin := "stack_chain_bin.bin"
	defer os.Remove(txt)
	defer os.Remove(bin)

	s := lr3.NewStack()
	s.Push("first")
	s.Push("second")

	// Сначала текст
	require.NoError(t, s.SaveText(txt))

	s2 := lr3.NewStack()
	require.NoError(t, s2.LoadText(txt))
	require.Equal(t, 2, s2.Size())

	// Потом бинарь из уже загруженного стека
	require.NoError(t, s2.SaveBinary(bin))

	s3 := lr3.NewStack()
	require.NoError(t, s3.LoadBinary(bin))
	require.Equal(t, 2, s3.Size())

	v1, _ := s3.Pop()
	v2, _ := s3.Pop()
	require.Equal(t, "second", v1)
	require.Equal(t, "first", v2)
}

func TestStackSaveLoadErrorPaths(t *testing.T) {
	s := lr3.NewStack()
	s.Push("x")

	// Ошибки при создании файлов
	err := s.SaveText("/root/stack_forbidden.txt")
	require.Error(t, err)

	err = s.SaveBinary("/root/stack_forbidden.bin")
	require.Error(t, err)

	// Ошибки при открытии файлов
	err = s.LoadText("/root/stack_forbidden.txt")
	require.Error(t, err)

	err = s.LoadBinary("/root/stack_forbidden.bin")
	require.Error(t, err)
}
