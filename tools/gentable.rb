#!ruby

def bitreflect(n, bitsize)
  m = 0
  for i in 0...bitsize
    m <<= 1
    m |= n[i]
  end
  m
end

#
# call-seq:
#   build_table(bitsize, polynomial, *opts) -> table
#
# [bitsize (integer)]
#
# [polynomial (integer)]
#
# [opts (hash)]
#
#   reflectin: true::
#
#   roundsize: 1::
#
#   slidewidth: 8::
#
#   align: :int:: nil, :byte, :int, or any positive integer
#
def build_table(bitsize, polynomial, reflectin: true, roundsize: 1, slidewidth: 8, align: nil)
  case align
  when nil
    align = bitsize
  when :byte
    align = (bitsize + 7) / 8 * 8
  when :int
    align = 8
    loop {
      break if bitsize <= align
      align <<= 1
    }
  when Integer
    # do nothing
  else
    raise "wrong align value - #{align} (expect nil, :byte, :int, or any positive integer)"
  end

  bitmask = ->(w) { ~(~0 << w) }
  padding = align - bitsize
  alignedmask = bitmask[align]
  times = 1 << slidewidth

  if reflectin
    adapt = ->(x) { bitreflect(x, bitsize) }
    shift = ->(x, w) { x >> w }
    slice = ->(x, off, w) { (x >> off) & bitmask[w] }
    input = ->(x) { x }
  else
    adapt = ->(x) { x << padding }
    shift = ->(x, w) { (x << w) & alignedmask }
    slice = ->(x, off, w) { (x >> (align - off - w)) & bitmask[w] }
    input = ->(x) { x << (align - 8) }
  end

  poly = adapt[polynomial]

  table = times.times.map do |r|
    r = input[r]
    slidewidth.times do
      if slice[r, 0, 1] == 0
        r = shift[r, 1]
      else
        r = shift[r, 1] ^ poly
      end
    end
    r
  end

  if roundsize > 1
    t = table
    table = [table, *Array.new(roundsize - 1) { [].clone }]
    times.times do |i|
      (1...roundsize).each do |j|
        r = table[j - 1][i]
        table[j][i] = (shift[r, slidewidth] ^ t[slice[r, 0, slidewidth]]) & alignedmask
      end
    end
  end

  table
end

def print_table(io, bitsize, table, tablename = "table", indent: 4)
  intbits = 8
  loop {
    break if intbits >= bitsize
    intbits <<= 1
  }

  intsize = intbits / 8
  inttype = "uint#{intbits}_t"

  case intbits
  when 8
    rowsize = 8
    format = "0x%02Xu"
  when 16
    rowsize = 4
    format = "0x%04Xu"
  when 32
    rowsize = 4
    format = "0x%08Xul"
  else
    rowsize = 2
    format = "0x%016Xull"
  end

  indentspace = " " * indent.to_i
  indent = ->(level, str = "") { indentspace * level.to_i + str }

  if table[0].is_a?(Array)
    io.puts "static const #{inttype} #{tablename}[#{table.size}][#{table[0].size}] =\n{"
    table.each_with_index do |t, i|
      io.puts indent[1, "{"]
      io.puts indent[2, "/* round: #{i + 1} */"]
      t.each_slice(rowsize) do |rr|
        io.puts indent[2, "#{rr.map { |r| format % r }.join(", ")},"]
      end
      io.puts indent[1, "},"]
    end
    io.puts "};"
  else
    io.puts "static const #{inttype} #{tablename}[#{table.size}] =\n{"
    table.each_slice(rowsize) do |rr|
      io.puts indent[1, "#{rr.map { |r| format % r }.join(", ")},"]
    end
    io.puts "};"
  end
end

ARGV.each do |arg|
  arg = arg.split(/\s*,\s*/)
  bitsize = arg[0].to_i
  polynomial = (arg[1] =~ /^0x/ ? arg[1].hex : arg[1].to_i)
  opts = {}
  (2...arg.size).step do |i|
    (a, *, b) = arg[i].partition("=")
    case a = a.intern
    when :reflectin
      case (b.downcase! || b)
      when "true"
        b = true
      when "false"
        b = false
      else
        raise "wrong reflectin value - #{b}"
      end
    when :roundsize
      b = b.to_i
    when :slidewidth
      b = b.to_i
    when :align
      case (b.downcase! || b)
      when "no", "none", "nil"
        b = nil
      when "byte", "char"
        b = :byte
      when "int", "integer"
        b = :int
      when /\A\d+\z/
        b = b.to_i
      else
        raise "wrong align value - #{b}"
      end
    else
      raise "not recognized option - #{a}"
    end

    opts[a] = b
  end

  print_table($stdout, bitsize, build_table(bitsize, polynomial, **opts), "table", indent: 4)
end
