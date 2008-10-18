def test_case
{"RawParseTree"=>
  [:block,
   [:lasgn, :var, [:lit, 2]],
   [:lasgn, :result, [:str, ""]],
   [:case,
    [:lvar, :var],
    [:when,
     [:array, [:lit, 1]],
     [:block,
      [:fcall, :puts, [:array, [:str, "something"]]],
      [:lasgn, :result, [:str, "red"]]]],
    [:when,
     [:array, [:lit, 2], [:lit, 3]],
     [:lasgn, :result, [:str, "yellow"]]],
    [:when, [:array, [:lit, 4]], nil],
    [:lasgn, :result, [:str, "green"]]],
   [:case,
    [:lvar, :result],
    [:when, [:array, [:str, "red"]], [:lasgn, :var, [:lit, 1]]],
    [:when, [:array, [:str, "yellow"]], [:lasgn, :var, [:lit, 2]]],
    [:when, [:array, [:str, "green"]], [:lasgn, :var, [:lit, 3]]],
    nil]],
 "Ruby"=>
  "var = 2\nresult = \"\"\ncase var\nwhen 1 then\n  puts(\"something\")\n  result = \"red\"\nwhen 2, 3 then\n  result = \"yellow\"\nwhen 4 then\n  # do nothing\nelse\n  result = \"green\"\nend\ncase result\nwhen \"red\" then\n  var = 1\nwhen \"yellow\" then\n  var = 2\nwhen \"green\" then\n  var = 3\nelse\n  # do nothing\nend\n",
 "ParseTree"=>
  s(:block,
   s(:lasgn, :var, s(:lit, 2)),
   s(:lasgn, :result, s(:str, "")),
   s(:case,
    s(:lvar, :var),
    s(:when,
     s(:array, s(:lit, 1)),
     s(:block,
      s(:call, nil, :puts, s(:arglist, s(:str, "something"))),
      s(:lasgn, :result, s(:str, "red")))),
    s(:when,
     s(:array, s(:lit, 2), s(:lit, 3)),
     s(:lasgn, :result, s(:str, "yellow"))),
    s(:when, s(:array, s(:lit, 4)), nil),
    s(:lasgn, :result, s(:str, "green"))),
   s(:case,
    s(:lvar, :result),
    s(:when, s(:array, s(:str, "red")), s(:lasgn, :var, s(:lit, 1))),
    s(:when, s(:array, s(:str, "yellow")), s(:lasgn, :var, s(:lit, 2))),
    s(:when, s(:array, s(:str, "green")), s(:lasgn, :var, s(:lit, 3))),
    nil))}
end
