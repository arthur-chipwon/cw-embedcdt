/*******************************************************************************
 * Copyright (c) 2013 Liviu Ionescu.
 *
 * This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License 2.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *     Liviu Ionescu - initial version
 *     Liviu Ionescu - UI part extraction.
 *******************************************************************************/

package org.eclipse.embedcdt.managedbuild.cross.riscv.core;

import org.eclipse.cdt.managedbuilder.core.BuildException;
import org.eclipse.cdt.managedbuilder.core.IConfiguration;
import org.eclipse.cdt.managedbuilder.core.IOption;
import org.eclipse.embedcdt.internal.managedbuild.cross.riscv.core.Activator;

public class Option {

	// ------------------------------------------------------------------------

	public static final String OPTION_PREFIX = "ilg.gnumcueclipse.managedbuild.cross.riscv.option";

	// public static final String OPTION_ARCHITECTURE = OPTION_PREFIX +
	// ".architecture";
	// public static final String ARCHITECTURE_RISCV = "riscv";

	public static final String OPTION_TARGET = OPTION_PREFIX + ".target.";

	public static final String OPTION_TARGET_ISA_BASE = OPTION_TARGET + "isa.base";
	public static final String OPTION_TARGET_ISA_FP = OPTION_TARGET + "isa.fp";
	public static final String OPTION_TARGET_ISA_ATOMIC = OPTION_TARGET + "isa.atomic";
	public static final String OPTION_TARGET_ISA_MULTIPLY = OPTION_TARGET + "isa.multiply";
	public static final String OPTION_TARGET_ISA_COMPRESSED = OPTION_TARGET + "isa.compressed";
	public static final String OPTION_TARGET_ISA_EXTENSIONS = OPTION_TARGET + "isa.extensions";

	public static final String OPTION_TARGET_ABI_INTEGER = OPTION_TARGET + "abi.integer";
	public static final String OPTION_TARGET_ABI_FP = OPTION_TARGET + "abi.fp";

	public static final String OPTION_TARGET_TUNE = OPTION_TARGET + "tune";
	public static final String OPTION_TARGET_CODEMODEL = OPTION_TARGET + "codemodel";
	public static final String OPTION_TARGET_SMALLDATALIMIT = OPTION_TARGET + "smalldatalimit";
	public static final String OPTION_TARGET_ALIGN = OPTION_TARGET + "align";
	public static final String OPTION_TARGET_SAVERESTORE = OPTION_TARGET + "saverestore";
	public static final String OPTION_TARGET_MEMCPY = OPTION_TARGET + "memcpy";

	// Deprecated since 2019-01-01
	public static final String OPTION_TARGET_PLT = OPTION_TARGET + "plt";
	public static final String OPTION_TARGET_FDIV = OPTION_TARGET + "fdiv";
	public static final String OPTION_TARGET_DIV = OPTION_TARGET + "div";

	public static final String OPTION_TARGET_OTHER = OPTION_TARGET + "other";

	public static final String OPTION_OPTIMIZATION = OPTION_PREFIX + ".optimization.";

	public static final String OPTION_OPTIMIZATION_LEVEL = OPTION_OPTIMIZATION + "level";
	public static final String OPTION_OPTIMIZATION_MESSAGELENGTH = OPTION_OPTIMIZATION + "messagelength";
	public static final String OPTION_OPTIMIZATION_SIGNEDCHAR = OPTION_OPTIMIZATION + "signedchar";
	public static final String OPTION_OPTIMIZATION_FUNCTIONSECTIONS = OPTION_OPTIMIZATION + "functionsections";
	public static final String OPTION_OPTIMIZATION_DATASECTIONS = OPTION_OPTIMIZATION + "datasections";
	public static final String OPTION_OPTIMIZATION_NOCOMMON = OPTION_OPTIMIZATION + "nocommon";
	public static final String OPTION_OPTIMIZATION_NOINLINEFUNCTIONS = OPTION_OPTIMIZATION + "noinlinefunctions";
	public static final String OPTION_OPTIMIZATION_FREESTANDING = OPTION_OPTIMIZATION + "freestanding";
	public static final String OPTION_OPTIMIZATION_NOBUILTIN = OPTION_OPTIMIZATION + "nobuiltin";
	public static final String OPTION_OPTIMIZATION_SPCONSTANT = OPTION_OPTIMIZATION + "spconstant";
	public static final String OPTION_OPTIMIZATION_PIC = OPTION_OPTIMIZATION + "PIC";
	public static final String OPTION_OPTIMIZATION_LTO = OPTION_OPTIMIZATION + "lto";
	public static final String OPTION_OPTIMIZATION_NOMOVELOOPINVARIANTS = OPTION_OPTIMIZATION + "nomoveloopinvariants";
	public static final String OPTION_OPTIMIZATION_OTHER = OPTION_OPTIMIZATION + "other";

	public static final String OPTION_WARNINGS = OPTION_PREFIX + ".warnings.";

	public static final String OPTION_WARNINGS_SYNTAXONLY = OPTION_WARNINGS + "syntaxonly";
	public static final String OPTION_WARNINGS_PEDANTIC = OPTION_WARNINGS + "pedantic";
	public static final String OPTION_WARNINGS_PEDANTICERRORS = OPTION_WARNINGS + "pedanticerrors";
	public static final String OPTION_WARNINGS_ALLWARN = OPTION_WARNINGS + "allwarn";
	public static final String OPTION_WARNINGS_NOWARN = OPTION_WARNINGS + "nowarn";
	public static final String OPTION_WARNINGS_EXTRAWARN = OPTION_WARNINGS + "extrawarn";
	public static final String OPTION_WARNINGS_CONVERSION = OPTION_WARNINGS + "conversion";
	public static final String OPTION_WARNINGS_UNINITIALIZED = OPTION_WARNINGS + "uninitialized";
	public static final String OPTION_WARNINGS_UNUSED = OPTION_WARNINGS + "unused";
	public static final String OPTION_WARNINGS_PADDED = OPTION_WARNINGS + "padded";
	public static final String OPTION_WARNINGS_FLOATEQUAL = OPTION_WARNINGS + "floatequal";
	public static final String OPTION_WARNINGS_SHADOW = OPTION_WARNINGS + "shadow";
	public static final String OPTION_WARNINGS_POINTERARITH = OPTION_WARNINGS + "pointerarith";
	public static final String OPTION_WARNINGS_LOGICALOP = OPTION_WARNINGS + "logicalop";
	public static final String OPTION_WARNINGS_AGREGGATERETURN = OPTION_WARNINGS + "agreggatereturn";
	public static final String OPTION_WARNINGS_MISSINGDECLARATION = OPTION_WARNINGS + "missingdeclaration";
	public static final String OPTION_WARNINGS_TOERRORS = OPTION_WARNINGS + "toerrors";
	public static final String OPTION_WARNINGS_OTHER = OPTION_WARNINGS + "other";

	public static final String OPTION_DEBUGGING = OPTION_PREFIX + ".debugging.";

	public static final String OPTION_DEBUGGING_LEVEL = OPTION_DEBUGGING + "level";
	public static final String OPTION_DEBUGGING_FORMAT = OPTION_DEBUGGING + "format";
	public static final String OPTION_DEBUGGING_PROF = OPTION_DEBUGGING + "prof";
	public static final String OPTION_DEBUGGING_GPROF = OPTION_DEBUGGING + "gprof";
	public static final String OPTION_DEBUGGING_OTHER = OPTION_DEBUGGING + "other";

	// other
	public static final String OPTION_TOOLCHAIN_ID = OPTION_PREFIX + ".toolchain.id";
	public static final String OPTION_TOOLCHAIN_NAME = OPTION_PREFIX + ".toolchain.name";

	// public static final String OPTION_TOOLCHAIN_PATH = OPTION_PREFIX
	// + ".toolchain.path";
	// public static final String OPTION_TOOLCHAIN_USE_GLOBAL_PATH =
	// OPTION_PREFIX
	// + ".toolchain.useglobalpath";
	// public static final boolean OPTION_TOOLCHAIN_USE_GLOBAL_PATH_DEFAULT =
	// true;

	public static final String OPTION_COMMAND = OPTION_PREFIX + ".command.";
	public static final String OPTION_COMMAND_PREFIX = OPTION_COMMAND + "prefix";
	public static final String OPTION_COMMAND_SUFFIX = OPTION_COMMAND + "suffix";
	public static final String OPTION_COMMAND_C = OPTION_COMMAND + "c";
	public static final String OPTION_COMMAND_CPP = OPTION_COMMAND + "cpp";
	public static final String OPTION_COMMAND_AR = OPTION_COMMAND + "ar";
	public static final String OPTION_COMMAND_OBJCOPY = OPTION_COMMAND + "objcopy";
	public static final String OPTION_COMMAND_OBJDUMP = OPTION_COMMAND + "objdump";
	public static final String OPTION_COMMAND_SIZE = OPTION_COMMAND + "size";
	public static final String OPTION_COMMAND_MAKE = OPTION_COMMAND + "make";
	public static final String OPTION_COMMAND_RM = OPTION_COMMAND + "rm";

	public static final String OPTION_PREFER_XPACKS_BIN = OPTION_PREFIX + ".preferxpacksbin";

	public static final String OPTION_ADDTOOLS = OPTION_PREFIX + ".addtools.";
	public static final String OPTION_ADDTOOLS_CREATEFLASH = OPTION_ADDTOOLS + "createflash";
	public static final String OPTION_ADDTOOLS_CREATELISTING = OPTION_ADDTOOLS + "createlisting";
	public static final String OPTION_ADDTOOLS_PRINTSIZE = OPTION_ADDTOOLS + "printsize";

	public static final String OPTION_CREATEFLASH_CHOICE = OPTION_PREFIX + ".createflash.choice";

	// These should be in sync with plugin.xml definitions
	public static final boolean OPTION_PREFER_XPACKS_BIN_DEFAULT = false;
	public static final boolean OPTION_ADDTOOLS_CREATEFLASH_DEFAULT = true;
	public static final boolean OPTION_ADDTOOLS_CREATELISTING_DEFAULT = false;
	public static final boolean OPTION_ADDTOOLS_PRINTSIZE_DEFAULT = true;

	public static final String CHOICE_IHEX = "ihex";
	public static final String CHOICE_SREC = "srec";
	public static final String CHOICE_SYMBOLSREC = "symbolsrec";
	public static final String CHOICE_BINARY = "binary";

	// ------------------------------------------------------------------------

	public static String getOptionStringValue(IConfiguration config, String sOptionId) {

		IOption option = config.getToolChain().getOptionBySuperClassId(sOptionId);
		String sReturn = null;
		if (option != null) {
			try {
				sReturn = option.getStringValue().trim();
			} catch (BuildException e) {
				Activator.log(e);
			}
		} else {
			Activator.log("string value " + sOptionId + " not found");
		}

		return sReturn;
	}

	public static String getOptionEnumCommand(IConfiguration config, String sOptionId) {

		IOption option = config.getToolChain().getOptionBySuperClassId(sOptionId);
		String sReturn = null;
		if (option != null) {
			try {
				String sEnumId = option.getStringValue();
				sReturn = option.getEnumCommand(sEnumId).trim();
			} catch (BuildException e) {
				Activator.log(e);
			}
		} else {
			Activator.log("enum command " + sOptionId + " not found");
		}

		return sReturn;
	}

	public static Boolean getOptionBooleanValue(IConfiguration config, String sOptionId) {

		IOption option = config.getToolChain().getOptionBySuperClassId(sOptionId);
		Boolean bReturn = null;
		if (option != null) {
			try {
				bReturn = Boolean.valueOf(option.getBooleanValue());
			} catch (BuildException e) {
				Activator.log(e);
			}
		} else {
			Activator.log("boolean value " + sOptionId + " not found");
		}

		return bReturn;
	}

	public static Boolean getOptionBooleanValue2(IConfiguration config, String sOptionId) {

		IOption option = config.getToolChain().getOptionBySuperClassId(sOptionId);
		Boolean bReturn = null;
		if (option != null) {
			try {
				IOption option2 = config.getToolChain().getOptionToSet(option, false);
				if (option2 == null) {
					return null;
				}
				bReturn = Boolean.valueOf(option.getBooleanValue());
			} catch (BuildException e) {
				Activator.log(e);
			}
		} else {
			Activator.log("boolean value2 " + sOptionId + " not found");
		}

		return bReturn;
	}

	public static String getOptionBooleanCommand(IConfiguration config, String sOptionId) {

		IOption option = config.getToolChain().getOptionBySuperClassId(sOptionId);
		String sReturn = null;
		if (option != null) {
			try {
				if (option.getBooleanValue()) {
					sReturn = option.getCommand().trim();
				} else {
					sReturn = option.getCommandFalse().trim();
				}
			} catch (BuildException e) {
				Activator.log(e);
			}
		} else {
			Activator.log("boolean command " + sOptionId + " not found");
		}

		return sReturn;
	}

	public static String getOptionStringCommandValue(IConfiguration config, String sOptionId) {

		IOption option = config.getToolChain().getOptionBySuperClassId(sOptionId);
		String sReturn = null;
		if (option != null) {
			try {
				String sValue = option.getStringValue();
				if (!sValue.isEmpty()) {
					sReturn = option.getCommand().trim() + sValue.trim();
				}
			} catch (BuildException e) {
				Activator.log(e);
			}
		} else {
			Activator.log("boolean command " + sOptionId + " not found");
		}

		return sReturn;
	}

	private static String getRiscvTargetIsa(IConfiguration config) {

		String sReturn = "";
		String sValue;

		String sIsa = getOptionEnumCommand(config, OPTION_TARGET_ISA_BASE);
		if (sIsa != null && sIsa.length() > 0) {
			sReturn += sIsa;

			if (!("-march=rv32g".equals(sIsa) || "-march=rv64g".equals(sIsa))) {
				// G equivalent to IMAFD
				sValue = getOptionBooleanCommand(config, OPTION_TARGET_ISA_MULTIPLY);
				if (sValue != null && !sValue.isEmpty()) {
					sReturn += sValue;
				}
				sValue = getOptionBooleanCommand(config, OPTION_TARGET_ISA_ATOMIC);
				if (sValue != null && !sValue.isEmpty()) {
					sReturn += sValue;
				}
				sValue = getOptionEnumCommand(config, OPTION_TARGET_ISA_FP);
				if (sValue != null && !sValue.isEmpty()) {
					sReturn += sValue;
				}
			}
			sValue = getOptionBooleanCommand(config, OPTION_TARGET_ISA_COMPRESSED);
			if (sValue != null && !sValue.isEmpty()) {
				sReturn += sValue;
			}
			sValue = getOptionStringValue(config, OPTION_TARGET_ISA_EXTENSIONS);
			if (sValue != null && !sValue.isEmpty()) {
				sReturn += sValue;
			}
		}

		return sReturn.trim();
	}

	private static String getRiscvTargetAbi(IConfiguration config) {

		String sReturn = "";
		String sValue;

		sValue = getOptionEnumCommand(config, OPTION_TARGET_ABI_INTEGER);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += sValue;

			sValue = getOptionEnumCommand(config, OPTION_TARGET_ABI_FP);
			if (sValue != null && !sValue.isEmpty()) {
				sReturn += sValue;
			}
		}

		return sReturn.trim();
	}

	private static String getOptimizationFlags(IConfiguration config) {

		String sReturn = "";
		String sValue;

		sValue = getOptionEnumCommand(config, OPTION_OPTIMIZATION_LEVEL);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_OPTIMIZATION_MESSAGELENGTH);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_OPTIMIZATION_SIGNEDCHAR);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_OPTIMIZATION_FUNCTIONSECTIONS);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_OPTIMIZATION_DATASECTIONS);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_OPTIMIZATION_NOCOMMON);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_OPTIMIZATION_NOINLINEFUNCTIONS);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_OPTIMIZATION_FREESTANDING);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_OPTIMIZATION_NOBUILTIN);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_OPTIMIZATION_SPCONSTANT);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_OPTIMIZATION_PIC);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_OPTIMIZATION_LTO);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_OPTIMIZATION_NOMOVELOOPINVARIANTS);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionStringValue(config, OPTION_OPTIMIZATION_OTHER);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		return sReturn.trim();
	}

	private static String getWarningFlags(IConfiguration config) {

		String sReturn = "";
		String sValue;

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_SYNTAXONLY);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_PEDANTIC);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_PEDANTICERRORS);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_NOWARN);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_TOERRORS);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_UNUSED);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_UNINITIALIZED);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_ALLWARN);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_EXTRAWARN);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_MISSINGDECLARATION);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_CONVERSION);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_POINTERARITH);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_PADDED);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_SHADOW);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_LOGICALOP);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_AGREGGATERETURN);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_WARNINGS_FLOATEQUAL);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionStringValue(config, OPTION_WARNINGS_OTHER);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		return sReturn.trim();
	}

	private static String getDebuggingFlags(IConfiguration config) {

		String sReturn = "";
		String sValue;

		sValue = getOptionEnumCommand(config, OPTION_DEBUGGING_LEVEL);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;

			sValue = getOptionEnumCommand(config, OPTION_DEBUGGING_FORMAT);
			if (sValue != null && !sValue.isEmpty()) {
				sReturn += " " + sValue;
			}
		}

		sValue = getOptionStringValue(config, OPTION_DEBUGGING_OTHER);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_DEBUGGING_PROF);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_DEBUGGING_GPROF);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		return sReturn.trim();
	}

	public static String getToolChainFlags(IConfiguration config) {
		String sReturn = "";
		String sValue;

		sValue = getRiscvTargetIsa(config);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getRiscvTargetAbi(config);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionEnumCommand(config, OPTION_TARGET_TUNE);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionEnumCommand(config, OPTION_TARGET_CODEMODEL);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionStringCommandValue(config, OPTION_TARGET_SMALLDATALIMIT);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionEnumCommand(config, OPTION_TARGET_ALIGN);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_TARGET_SAVERESTORE);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionBooleanCommand(config, OPTION_TARGET_MEMCPY);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptionStringValue(config, OPTION_TARGET_OTHER);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getOptimizationFlags(config);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getWarningFlags(config);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		sValue = getDebuggingFlags(config);
		if (sValue != null && !sValue.isEmpty()) {
			sReturn += " " + sValue;
		}

		return sReturn.trim();
	}

	// ------------------------------------------------------------------------
}
