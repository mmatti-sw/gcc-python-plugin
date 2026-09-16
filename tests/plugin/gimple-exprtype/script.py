#   Copyright 2026 Manjunath Matti <mmatti@linux.ibm.com>
#
#   This is free software: you can redistribute it and/or modify it
#   under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#   General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see
#   <http://www.gnu.org/licenses/>.

# Verify gcc.Gimple.exprtype for the various kinds of statement.
#
# Only the statement kind and the type are printed: the SSA names within
# a statement vary between GCC releases, the types do not.

import gcc

def on_pass_execution(p, fn):
    if p.name == '*warn_function_return' and fn.decl.name == 'test_fn':
        seen = set()
        for bb in fn.cfg.basic_blocks:
            if not bb.gimple:
                continue
            for stmt in bb.gimple:
                # GCC 16 spells the boolean type "bool" (as C23 does),
                # earlier releases spell it "_Bool":
                typename = str(stmt.exprtype)
                if typename == '_Bool':
                    typename = 'bool'
                seen.add('%s: %s' % (type(stmt).__name__, typename))
        for line in sorted(seen):
            print(line)

gcc.register_callback(gcc.PLUGIN_PASS_EXECUTION,
                      on_pass_execution)
