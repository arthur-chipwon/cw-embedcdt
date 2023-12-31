/*******************************************************************************
 * Copyright (c) 2014 Liviu Ionescu.
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
 *******************************************************************************/

package org.eclipse.embedcdt.debug.gdbjtag.qemu.core.dsf;

import org.eclipse.cdt.dsf.concurrent.RequestMonitor;
import org.eclipse.cdt.dsf.service.DsfSession;
import org.eclipse.core.runtime.CoreException;
import org.eclipse.core.runtime.IStatus;
import org.eclipse.core.runtime.Status;
import org.eclipse.debug.core.ILaunchConfiguration;
import org.eclipse.embedcdt.core.StringUtils;
import org.eclipse.embedcdt.debug.gdbjtag.core.dsf.GnuMcuGdbServerBackend;
import org.eclipse.embedcdt.debug.gdbjtag.qemu.core.Configuration;
import org.eclipse.embedcdt.internal.debug.gdbjtag.qemu.core.Activator;
import org.osgi.framework.BundleContext;

public class GdbServerBackend extends GnuMcuGdbServerBackend {

	// ------------------------------------------------------------------------

	protected int fGdbServerLaunchTimeout = 15;

	// ------------------------------------------------------------------------

	public GdbServerBackend(DsfSession session, ILaunchConfiguration lc) {
		super(session, lc);

		if (Activator.getInstance().isDebugging()) {
			System.out.println("qemu.GdbServerBackend(" + session + "," + lc.getName() + ")");
		}
	}

	// ------------------------------------------------------------------------

	@Override
	public void initialize(final RequestMonitor rm) {

		if (Activator.getInstance().isDebugging()) {
			System.out.println("qemu.GdbServerBackend.initialize()");
		}

		try {
			// Update parent data member before calling initialise.
			fDoStartGdbServer = Configuration.getDoStartGdbServer(fLaunchConfiguration);
		} catch (CoreException e) {
			rm.setStatus(new Status(IStatus.ERROR, Activator.PLUGIN_ID, -1, "Cannot get configuration", e)); //$NON-NLS-1$
			rm.done();
			return;
		}

		// Initialise the super class, and, when ready, perform the local
		// initialisations.
		super.initialize(new RequestMonitor(getExecutor(), rm) {

			@Override
			protected void handleSuccess() {
				doInitialize(rm);
			}
		});
	}

	private void doInitialize(RequestMonitor rm) {

		if (Activator.getInstance().isDebugging()) {
			System.out.println("qemu.GdbServerBackend.doInitialize()");
		}
		rm.done();
	}

	@Override
	public void shutdown(final RequestMonitor rm) {

		if (Activator.getInstance().isDebugging()) {
			System.out.println("qemu.GdbServerBackend.shutdown()");
		}

		super.shutdown(rm);
	}

	@Override
	public void destroy() {

		if (Activator.getInstance().isDebugging()) {
			System.out.println("qemu.GdbServerBackend.destroy() " + Thread.currentThread());
		}

		// Destroy the parent (the GDB server; the client is also destroyed
		// there).
		super.destroy();
	}

	// ------------------------------------------------------------------------

	@Override
	protected BundleContext getBundleContext() {
		return Activator.getInstance().getBundle().getBundleContext();
	}

	@Override
	public String[] getServerCommandLineArray() {
		String[] commandLineArray = Configuration.getGdbServerCommandLineArray(fLaunchConfiguration);

		return commandLineArray;
	}

	@Override
	public String getServerCommandName() {

		String[] commandLineArray = getServerCommandLineArray();
		if (commandLineArray == null) {
			return null;
		}

		String fullCommand = commandLineArray[0];
		return StringUtils.extractNameFromPath(fullCommand);
	}

	@Override
	public int getServerLaunchTimeoutSeconds() {
		return fGdbServerLaunchTimeout;
	}

	@Override
	public String getServerName() {
		return "QEMU";
	}

	@Override
	public boolean canMatchStdOut() {
		return Configuration.hasLegacyInterface(fLaunchConfiguration);
	}

	@Override
	public boolean canMatchStdErr() {
		return false;
	}

	@Override
	public boolean matchStdOutExpectedPattern(String line) {
		if (line.indexOf("GDB Server listening on:") >= 0) {
			return true;
		}

		return false;
	}

	/**
	 * Use the exit code and the captured
	 * string to compose the text displayed in case of error.
	 *
	 * @param exitCode
	 *            an integer with the process exit code.
	 * @param message
	 *            a string with the captured stderr text.
	 * @return a string with the text to be displayed.
	 */
	@Override
	public String prepareMessageBoxText(int exitCode) {

		String body = "";

		String name = getServerCommandName();
		if (name == null) {
			name = "GDB Server";
		}
		String tail = "\n\nFor more details, see the " + name + " console.";

		if (body.isEmpty()) {
			return getServerName() + " failed with code (" + exitCode + ")." + tail;
		} else {
			return getServerName() + " failed: \n" + body + tail;
		}
	}

	// ------------------------------------------------------------------------
}
