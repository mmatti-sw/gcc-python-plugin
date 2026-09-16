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

# Verify that gcc.CallgraphNode and gcc.Variable wrappers keep the
# underlying GCC objects alive across a garbage collection, i.e. that
# gcc_cgraph_node_mark_in_use and gcc_variable_mark_in_use mark them.

import gcc

nodes = []
variables = []

def on_pass_execution(p, fn):
    global nodes, variables
    if p.name == '*free_lang_data':
        nodes = gcc.get_callgraph_nodes()
        variables = gcc.get_variables()

def on_finish():
    # Collect with the wrappers still referenced from Python, then use
    # the objects they wrap:
    gcc._force_garbage_collection()
    print('callgraph nodes: %s'
          % sorted(str(node.decl.name) for node in nodes))
    print('variables: %s'
          % sorted(str(var.decl.name) for var in variables))

gcc.register_callback(gcc.PLUGIN_PASS_EXECUTION,
                      on_pass_execution)
gcc.register_callback(gcc.PLUGIN_FINISH,
                      on_finish)
