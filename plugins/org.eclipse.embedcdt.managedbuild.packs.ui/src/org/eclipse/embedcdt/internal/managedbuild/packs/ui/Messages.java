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
 *     Liviu Ionescu - UI part extraction.
 *******************************************************************************/

package org.eclipse.embedcdt.internal.managedbuild.packs.ui;

import org.eclipse.osgi.util.NLS;

public class Messages extends NLS {

	private static final String MESSAGES = "org.eclipse.embedcdt.internal.managedbuild.packs.ui.messages"; //$NON-NLS-1$

	public static String DevicesTab_DeviceGroup_name;
	public static String DevicesTab_DeviceGroup_architecture_label;
	public static String DevicesTab_MemoryGroup_name;
	public static String DevicesTab_MemoryGroup_editButton_text;

	public static String DevicesTab_MemoryGroup_nameColumn_toolTipText;
	public static String DevicesTab_MemoryGroup_startColumn_toolTipText;
	public static String DevicesTab_MemoryGroup_sizeColumn_toolTipText;
	public static String DevicesTab_MemoryGroup_startupColumn_toolTipText;

	static {
		// initialise resource bundle
		NLS.initializeMessages(MESSAGES, Messages.class);
	}

	private Messages() {
	}

}
