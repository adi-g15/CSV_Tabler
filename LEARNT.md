### LEARNT
----

1. Using a pointer to a class object of which the class is declared later, but you cant define it before, since that too uses a pointer to this class... this was a problem i faced in the Attendance_Register
Solution - We can just 'declare' a structure, ie. that there will be a structure with so and so name... i really didn't knew before that this is possible ;D
        Note- But it shows "Class_name has Incomplete type", if you use it with a name in a method, but havent defined the class by then

2. We can access even the private members of a passed reference/object of the same class if using any method
    Seems obvious, but i didnt knew it, and then thought that this should be obvious bcz that's how copy constructors may work

3. Use 'ch = getchar();' So that '\n' isn't ignored, but spaces are ignored
