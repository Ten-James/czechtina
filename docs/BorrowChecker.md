# Borrow Checker

Dynamic memory can be saved in two ways.

As basic pointer to some array or pointer with ownership to memory.

## Czechtina approach

### For basic variables

variable defined as `&Any` should contain some pointer to heap memory and its gonna be deallocated as the scope gets pop.
For moving the memory out of the variable ownership, use `&<var>` in any assigment or parameter to function call.
Since the variable memory is moved out of the variable, it should not be used anymore as the behavior might be undefined.

### For structure props

#### Hard memory ownership

If prop type is `&Any` the memory has to be allocated in constructor, and its deallocated with destructor
or with DestructInto

#### Soft memory ownership

if prop type is `&?Any` or `?&Any` (the order doesn't matter), The value might be `None` and DestructInto returns `?&Any`.

#### DestructInto behavior

Since the struct owns the memory, to get ownership of the prop. when DestructInto is called with propName parameter,
the memory ownership is moved and the struct is destructed.
