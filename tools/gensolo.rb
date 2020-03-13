#!ruby

[
  [ 2, "Sexdecuple"],
  [ 4, "Duotriguple"],
  [ 8, "Quattuorsexaguple"],
  [16, "Octovigcentuple"],
  [32, "Sexquinquagducentuple"],
].each do |(bytes, tuple)|
  name = "CRCEA_UPDATE_BY#{bytes}_SOLO"
  decl = "CRCEA_BY#{bytes}_SOLO_DECL"
  round = bytes * 8

  code = ""
  code << <<-CODE1

/*
 * Slicing by #{tuple} Solo
 */
CRCEA_VISIBILITY CRCEA_INLINE CRCEA_TYPE
#{name}(const crcea_design *design, const char *p, const char *pp, CRCEA_TYPE state, const void *table)
{
    const CRCEA_TYPE (*t)[2] = (const CRCEA_TYPE (*)[2])table;

#define #{decl}(SETUP_POLYNOMIAL, INPUT, SHIFT, SLICE, SLICE8, LOAD16, INDEX16) \\
    CRCEA_UPDATE_STRIPE(p, pp, #{bytes}); \\
  CODE1

  bwid = Math.log(bytes + 1, 10).ceil
  rwid = Math.log(round + 1, 10).ceil
  bytes.times do |i|
    code << <<-CODE2 % [bwid, i, bwid, i, rwid, i * 8]
        const uint8_t n%-*s = (uint8_t)p[%*s] ^ SLICE(state, %*s, 8); \\
    CODE2
  end

  code << <<-CODE3
        state = SHIFT(state, #{bytes * 8}) ^ \\
  CODE3

  round.times do |r|
    code << <<-CODE4 % [rwid, round - r - 1, r / 8, " " * (bwid - (r / 8).to_s.size), r % 8]
                t[%*s][SLICE8(n%s,%s %s, 1)] ^ \\
    CODE4
  end

  code << <<-CODE5
    CRCEA_UPDATE_BYTE(p, pp); \\
        state ^= INPUT(*p); \\
        const uint8_t n = SLICE(state, 0, 8); \\
        state = SHIFT(state, 8) ^ \\
                t[7][SLICE8(n, 0, 1)] ^ \\
                t[6][SLICE8(n, 1, 1)] ^ \\
                t[5][SLICE8(n, 2, 1)] ^ \\
                t[4][SLICE8(n, 3, 1)] ^ \\
                t[3][SLICE8(n, 4, 1)] ^ \\
                t[2][SLICE8(n, 5, 1)] ^ \\
                t[1][SLICE8(n, 6, 1)] ^ \\
                t[0][SLICE8(n, 7, 1)]; \\
    CRCEA_UPDATE_END(); \\

    CRCEA_UPDATE_DECL(design, state, #{decl});

    return state;
}
  CODE5

  puts code
end
