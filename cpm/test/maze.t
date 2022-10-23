! Maze program in T3X/Z for VT100/220/240 terminals.
!
! Builds a maze and then solves it. Tested on a P112 CPU board (16MHz Z182).
! Looks great on a linux xterm with a dark background.
! The delay_msec() routine may require adjusting to match your CPU speed.
!
! H. Peraza, Jan 2020.
!
! Changelog:
!
! 27.01.2020 - Pass a seed value via command line - Nils M. Holm
! 31.01.2020 - Take the check_abort() call out of the inner loop in
!              delay_msec() - Martin

const FALSE = 0, TRUE = %1;

const ROWS  = 11;  ! cells are 2 chars tall
const COLS  = 26;  ! and 3 chars wide on the screen

var rseed, cells, start, moves, best_moves, aborted;

var w[ROWS], wb::ROWS*COLS, v[ROWS], vb::ROWS*COLS, d::4;

! Console I/O routines:

! Output character,
! use BDOS fn 6 to prevent possible character filtering of BDOS fn 2.
putc(c) do
    t.bdos(6, c);
end

! Check for abort key
check_abort() do
    ! any key aborts
    if (t.bdos(6, 0xff) \= 0) aborted := TRUE;
end

! Output string
put_str(str) do var i;
    i := 0;
    while (str::i) do
        putc(str::i);
        i := i+1;
    end
end

! Output value as unsigned decimal, no leading zeros
put_dec(val) do
    var digit, divisor, suppress;

    suppress := TRUE;
    divisor := 10000;
    while (divisor > 1) do
        digit := val / divisor;
        ie (digit = 0)
            if (\suppress) putc('0');
        else do
            putc(digit + '0');
            suppress := FALSE;
        end
        val := val mod divisor;
        divisor := divisor / 10;
    end
    putc(val + '0');
end

! Terminal-specific routines, VT100:

! Clear screen
cls() do
    put_str("\e[2J");
end

! Position cursor
set_cur(x, y) do
    putc('\e');
    putc('[');
    put_dec(y + 1);
    putc(';');
    put_dec(x + 1);
    putc('H');
end

! Switch to graphics mode
graph_mode() do
    put_str("\e(0\e)0");  ! set both GS0 and GS1 just in case
end

! Switch to alphanumeric mode
alpha_mode() do
    put_str("\e(B\e)B");  ! reset both GS0 and GS1
end

! Turn on bold attribute
bold_mode() do
    put_str("\e[1m");
end

! Turn off all attributes
attrib_off() do
    put_str("\e[0m");
end

! Turn off cursor
cursor_off() do
    put_str("\e[?25l");
end

! Turn on cursor
cursor_on() do
    put_str("\e[?25h");
end

! Set color (VT240)
set_color(fg, bg) do
    ie (fg = -1) do
        ! background color only
        putc('\e');
        putc('[');
        put_dec(bg + 40);
        putc('m');
    end else ie (bg = -1) do
        ! foreground color only
        putc('\e');
        putc('[');
        put_dec(fg + 30);
        putc('m');
    end else do
        ! both
        putc('\e');
        putc('[');
        put_dec(fg + 30);
        putc(';');
        put_dec(bg + 40);
        putc('m');
    end
end

! Simple random number generator
rnd(x) do
    rseed := rseed * 61069;
    rseed := rseed + 1;
    return (rseed >> 8) mod x;
end

! Delay routines
! *** adjust j loop value to match your CPU speed ***
! Value given for a 4MHz Z80 system.
delay_msec(msec) do var i, j, k;
    for (i = 0, msec) do
        check_abort();
        if (aborted) return;
        for (j = 0, 20) do
            k := i + j;  ! do something to kill time
        end
    end
end

delay_sec(seconds) do var i;
    for (i = 0, seconds)
        delay_msec(1000);
end

! Build a maze.
! The maze contruction algorithm was taken from the well-known
! Creative Computing BASIC maze program.
build_maze() do var r, c, b, i, n;

    t.memfill(d, 0, 4);

    t.memfill(wb, 0, cells);  ! initially all cells have
    t.memfill(vb, 0, cells);  ! bottom and right walls

    b := FALSE;
    start := rnd(COLS);

    r := 0;
    c := start;
    n := 1;
    w[r]::c := 1;  ! entry on first row

    while (TRUE) do

        ! check for empty cells around the current location
        ! and create a path vector accordingly

        i := 0;
        if (c > 0) do
            if (w[r]::(c-1) = 0) do
                d::i := 1;  ! left
                i := i+1;
            end
        end

        if (c < COLS-1) do
            if (w[r]::(c+1) = 0) do
                d::i := 2;  ! right
                i := i+1;
            end
        end

        if (r > 0) do
            if (w[r-1]::c = 0) do
                d::i := 3;  ! up
                i := i+1;
            end
        end

        ie (r < ROWS-1) do
            if (w[r+1]::c = 0) do
                d::i := 4;  ! down
                i := i+1;
            end
        end else if (\b) do
            ! only one exit in the last row
            d::i := 4;      ! down
            i := i+1;
        end

        if (i > 0) do
            ! now, pick randomly one of the available directions
            ! from the path vector to move to

            i := d::rnd(i);

            ie (i = 4) do
                ! down
                ! clear the bottom wall on this cell and move down
                v[r]::c := v[r]::c + 1;
                r := r + 1;
                if (r >= ROWS) do
                    ! we execute this part only once!
                    b := TRUE;  ! only one exit on the last row
                    r := 0;  ! restart scanning from the beginning
                    c := 0;
                    ! find an occupied cell and start from there
                    while (w[r]::c = 0) do
                        c := c + 1;
                        if (c >= COLS) do
                            c := 0;
                            r := r + 1;
                            if (r >= ROWS) r := 0;
                        end
                    end
                    loop;
                end

            end else ie (i = 3) do
                ! up
                r := r - 1;
                ! clear the bottom wall on the upper cell
                v[r]::c := 1;

            end else ie (i = 2) do
                ! right
                ! clear the right wall on this cell and move right
                v[r]::c := v[r]::c + 2;
                c := c + 1;

            end else do
                ! left
                c := c - 1;
                ! clear the right wall of the previous cell
                v[r]::c := 2;

            end

            n := n + 1;
            ! mark cell as occupied in the aux array
            w[r]::c := 1;

            ! keep going until we have scanned the whole thing
            if (n >= cells) return;

            loop;

        end  ! if (i > 0)

        ! there were no cells we could move to
        ! thus scan the array and try continuing from the next occupied cell

        while (TRUE) do
            c := c + 1;
            if (c >= COLS) do
                c := 0;
                r := r + 1;
                if (r >= ROWS) r := 0;
            end
            if (w[r]::c \= 0) leave;
        end

    end  ! while (TRUE)

end ! build_maze

! Draw single cell
draw_cell(r, c) do var b, str;

    set_cur(c*3+1, r*2+1);

    putc(' ');
    putc(' ');  ! always empty
    ie ((v[r]::c & 2) = 0)     ! right wall?
        putc('x');
    else
        putc(' ');

    set_cur(c*3+1, r*2+2);
    ie ((v[r]::c & 1) = 0) do  ! bottom wall?
        putc('q');
        putc('q');
    end else do
        putc(' ');
        putc(' ');
    end

    ! this is the tricky one: find a graphic character that connects
    ! nicely with the surrounding ones
    b := v[r]::c & 3;
    ie (c < COLS - 1)
        b := b | ((v[r]::(c+1) << 2) & 4);
    else
        b := b | 4;
    ie (r < ROWS-1)
        b := b | ((v[r+1]::c << 2) & 8);
    else
        b := b | 8;
    str := " qxjqqmvxkxulwtn";
    putc(str::(~b & 15));

end

! Draw the maze
draw_maze() do var r, c;

    attrib_off();
    cls();

    graph_mode();
    set_color(6, -1);

    ! top row: continuous wall with just a single entry

    set_cur(0, 0);
    putc('l');
    for (c = 0, COLS) do
        set_cur(c*3+1, 0);
        ie (c = start) do
            putc(' ');
            putc(' ');
        end else do
            putc('q');
            putc('q');
        end
        ie (c < COLS-1) do
            ie ((v[0]::c & 2) = 0)
                putc('w');
            else
                putc('q');
        end else do
            putc('k');
        end
    end

    ! remaining rows: there is always a leftmost wall,
    ! right and bottom cell walls are drawn according
    ! to the array elements

    for (r = 0, ROWS) do
        set_cur(0, r*2+1);
        putc('x');
        set_cur(0, r*2+2);
        ie (r < ROWS-1) do
            ie ((v[r]::0 & 1) = 0)
                putc('t');
            else
                putc('x');
        end else do
            putc('m');
        end
        for (c = 0, COLS)
            draw_cell(r, c);
    end

    attrib_off();
    alpha_mode();

end

! Draw the blob that travels thru the maze
draw_blob(r, c, b, dir) do

    set_cur(c*3+1, r*2+1);
    putc(b);
    putc(b);

    ie (dir = 1) do
        ! left (coming from right)
        set_cur(c*3+3, r*2+1);
        putc(b);

    end else ie (dir = 2) do
        ! right
        set_cur(c*3, r*2+1);
        putc(b);

    end else ie (dir = 3) do
        ! up
        set_cur(c*3+1, r*2+2);
        putc(b);
        putc(b);

    end else if (dir = 4) do
        ! down
        set_cur(c*3+1, r*2);
        putc(b);
        putc(b);

    end

    delay_msec(50);

end

! Find our way out of the current cell, then call again recursively
! until we find the exit door.
check_move(r, c) do

    ! are we at the exit door?
    if (r = ROWS-1 /\ (v[r]::c & 1) \= 0) do
        ! if so, terminate
        draw_blob(r+1, c, 'a', 4);
        return TRUE;
    end

    ! see if we can move down
    if (r < ROWS-1) do
        if ((v[r]::c & 1) \= 0 /\ w[r+1]::c = 0) do
            ! yes, and we haven't been here either
            r := r + 1;
            w[r]::c := 1;
            moves := moves + 1;
            best_moves := best_moves + 1;
            draw_blob(r, c, 'a', 4);
            if (check_move(r, c)) return TRUE;
            ! back up
            draw_blob(r, c, ' ', 4);
            w[r]::c := -1;  ! no way out
            r := r - 1;
            best_moves := best_moves - 1;
        end
    end

    ! see if we can go left
    if (c > 0) do
        if ((v[r]::(c-1) & 2) \= 0 /\ w[r]::(c-1) = 0) do
            ! yes, and we haven't been here either
            c := c - 1;
            w[r]::c := 1;
            moves := moves + 1;
            best_moves := best_moves + 1;
            draw_blob(r, c, 'a', 1);
            if (check_move(r, c)) return TRUE;
            ! back up
            draw_blob(r, c, ' ', 1);
            w[r]::c := -1;  ! no way out
            c := c + 1;
            best_moves := best_moves - 1;
        end
    end

    ! try right
    if (c < COLS-1) do
        if ((v[r]::c & 2) \= 0 /\ w[r]::(c+1) = 0) do
            c := c + 1;
            w[r]::c := 1;
            moves := moves + 1;
            best_moves := best_moves + 1;
            draw_blob(r, c, 'a', 2);
            if (check_move(r, c)) return TRUE;
            ! back up
            draw_blob(r, c, ' ', 2);
            w[r]::c := -1;  ! no way out
            c := c - 1;
            best_moves := best_moves - 1;
        end
    end

    ! try up as last resort
    if (r > 0) do
        if ((v[r-1]::c & 1) \= 0 /\ w[r-1]::c = 0) do
            r := r - 1;
            w[r]::c := 1;
            moves := moves + 1;
            best_moves := best_moves + 1;
            draw_blob(r, c, 'a', 3);
            if (check_move(r, c)) return TRUE;
            ! back up
            draw_blob(r, c, ' ', 3);
            w[r]::c := -1;  ! no way out
            r := r + 1;
            best_moves := best_moves - 1;
        end
    end

    return FALSE;
end

! Solve the maze
solve() do var r, c, s, n;

    graph_mode();
    cursor_off();
    bold_mode();
    set_color(3, 1);

    t.memfill(wb, 0, cells);

    moves := 0;
    best_moves := 0;

    ! we start at the input door
    r := 0;
    c := start;

    w[r]::c := 1;
    draw_blob(r, c, 'a', 4);

    s := check_move(r, c);

    attrib_off();
    alpha_mode();
    cursor_on();

    set_cur(0, 23);
    ie (s) do
        ! solution found
        put_str("Solved in ");
        put_dec(moves);
        put_str(" moves, shortest path = ");
        put_dec(best_moves);
        put_str(" moves. ");
    end else do
        ! should never happen...
        put_str("No solution found!!! ");
    end

end

! Convert ASCII decimal value to binary
aton(s) do var n;
    n := 0;
    while ('0' <= s::0 /\ s::0 <= '9') do
        n := n * 10 + s::0 - '0';
        s := s+1;
    end;
    return n;
end

! Main program
do var i, arg::10;

    ie (t.getarg(1, arg, 10) > 0)
        rseed := aton(arg);
    else
        rseed := 22095;

    cells := ROWS*COLS;
    aborted := FALSE;

    for (i = 0, ROWS) do
        w[i] := @wb::(i*COLS);
        v[i] := @vb::(i*COLS);
    end

    while (\aborted) do
        build_maze();
        draw_maze();
        delay_sec(1);
        solve();
        delay_sec(5);
        check_abort();
    end

    attrib_off();
    alpha_mode();
    cursor_on();

    set_cur(0, 23);
    halt 0;
end
