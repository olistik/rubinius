def test_case
{"RawParseTree"=>[:defn, :empty, [:scope, [:block, [:args], [:nil]]]],
 "Ruby"=>"def empty\n  # do nothing\nend",
 "ParseTree"=>s(:defn, :empty, s(:args), s(:scope, s(:block, s(:nil))))}
end
