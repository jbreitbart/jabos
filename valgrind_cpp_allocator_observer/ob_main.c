#include "pub_tool_basics.h"	// needed by every tool
#include "pub_tool_tooliface.h" // needed by every tool
#include "pub_tool_libcprint.h" // for printing functions
#include "pub_tool_machine.h"   // for VG_(fnptr_to_fnentry)

// statistics on number of guest instructions executed
static ULong n_guest_instrs = 0;

// Functions to be called by instrumentation.
//
// In Valgrind terminology, these are called "dirty helpers"
// (dirty because they can have side effects).
// To add an instruction for calling these helpers, use e.g.
//
//   IRExpr** argv = mkIRExprVec_2(Par1, Par2);
//   IRDirty* di = unsafeIRDirty_0_N(
//       2, /* regparms - accelerates parameter passing on x86 */
//       "helperName",
//       VG_(fnptr_to_fnentry)(helperFunction), argv);
//   addStmtToIRSB(bb, IRStmt_Dirty(di));
//
// For more details about the intermediate representation (IR)
// used by Valgrind as virtual instruction set (called "VEX"),
// such as types, operations, statements, factory functions,
// see "../VEX/pub/libvex_ir.h", or "../lackey/lk_main.c" for
// example usage.

// helper to increment statistics counter
extern int dummy(void);
static void inc_guest_instr(void) {
	dummy();
	n_guest_instrs++;
}

// helper to write information about guest memory stores
//
// VG_REGPARM(N): pass N (up to 3) arguments in registers on x86 --
// more efficient than via stack. Ignored on other architectures.
static VG_REGPARM(2) void trace_store(Addr addr, SizeT size) { VG_(printf)("Store : %08lx, %lu\n", addr, size); }

// Instrumentation function. "sbIn" is the code block passed in.
// Other arguments are more obscure and often not needed -- see
// include/pub_tool_tooliface.h.
static IRSB *ob_instrument(VgCallbackClosure *closure, IRSB *sbIn, VexGuestLayout *layout, VexGuestExtents *vge,
						   IRType gWordTy, IRType hWordTy) {
	Int i;
	IRSB *sbOut;

	/* Set up a copy of the input SB without the statements */
	sbOut = deepCopyIRSBExceptStmts(sbIn);

	// Copy verbatim any IR preamble preceding the first IMark
	i = 0;
	while (i < sbIn->stmts_used && sbIn->stmts[i]->tag != Ist_IMark) {
		addStmtToIRSB(sbOut, sbIn->stmts[i]);
		i++;
	}

	for (/*use current i*/; i < sbIn->stmts_used; i++) {
		IRStmt *st = sbIn->stmts[i];
		if (!st || st->tag == Ist_NoOp) continue;

		// Any special instrumentation goes here
		switch (st->tag) {
		case Ist_IMark: {
			// TODO: call to inc_guest_instr (see comment above!)
			IRDirty *di =
				unsafeIRDirty_0_N(0, "inc_guest_instr", VG_(fnptr_to_fnentry)(inc_guest_instr), mkIRExprVec_0());
			addStmtToIRSB(sbOut, IRStmt_Dirty(di));

		} break;
		case Ist_Store: {
			// TODO: call to trace_store with appropriate parameters.
			// * use "st->Ist.Store.addr" for par1 (type is correct: IRExpr*)
			// * use sizeofIRType(typeOfIRExpr(sbIn->tyenv, ... <IRExpr*> ))
			//   to get the length of the data access for par2 as "Int".
			//   Parameters need to be passed as IRExpr*, using IR type HWord.
			//   Use mkIRExpr_HWord() to convert from Int to IRExpr*
		} break;
		default:
			break;
		}

		// always pass through original instruction
		addStmtToIRSB(sbOut, st);
	}

	return sbOut;
}

// Finalization function
static void ob_fini(Int exitcode) { VG_(umsg)("Guest instructions executed: %'llu\n", n_guest_instrs); }

// Post-option-processing initialization function
static void ob_post_clo_init(void) {}

// Pre-option-processing initialization function
static void ob_pre_clo_init(void) {
	// Required details for start-up message
	VG_(details_name)("Observer");
	VG_(details_version)(NULL);
	VG_(details_description)("an execution observer");
	VG_(details_copyright_author)("Copyright (C) 2011, VT Hacker");
	// Required detail for crash message
	VG_(details_bug_reports_to)("/dev/null");
	// Name the required functions #2, #3 and #4.
	VG_(basic_tool_funcs)(ob_post_clo_init, ob_instrument, ob_fini);
}

// This prevents core/tool interface problems, and names the required
// function #1, giving the core an entry point into the tool.
VG_DETERMINE_INTERFACE_VERSION(ob_pre_clo_init)
