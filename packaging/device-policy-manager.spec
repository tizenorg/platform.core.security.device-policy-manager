Name:    device-policy-manager
Version: 0.0.1
Release: 0
License: Apache-2.0
Source0: file://%{name}-%{version}.tar.gz
Summary: Tizen Device Policy Manager
Group:   Security/Other
BuildRequires: gcc
BuildRequires: cmake
BuildRequires: pam-devel
BuildRequires: gettext-tools
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(libxml-2.0)
BuildRequires: pkgconfig(sqlite3)
BuildRequires: pkgconfig(bundle)
BuildRequires: pkgconfig(pkgmgr)
BuildRequires: pkgconfig(pkgmgr-info)
BuildRequires: pkgconfig(aul)
BuildRequires: pkgconfig(appsvc)
BuildRequires: pkgconfig(syspopup-caller)
BuildRequires: pkgconfig(deviced)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(vconf-internal-keys)
BuildRequires: pkgconfig(bluetooth-api)
BuildRequires: pkgconfig(capi-network-bluetooth)
BuildRequires: pkgconfig(libsmack)
BuildRequires: pkgconfig(libtzplatform-config)
BuildRequires: pkgconfig(security-privilege-manager)
BuildRequires: pkgconfig(capi-base-common)
BuildRequires: pkgconfig(capi-system-info)
BuildRequires: pkgconfig(capi-network-wifi)
BuildRequires: pkgconfig(capi-network-connection)
BuildRequires: pkgconfig(capi-network-bluetooth)
BuildRequires: pkgconfig(capi-system-system-settings)
BuildRequires: pkgconfig(notification)
BuildRequires: pkgconfig(key-manager)

%if "%{profile}" != "tv"
BuildRequires: pkgconfig(capi-location-manager)
BuildRequires: pkgconfig(auth-fw-admin)
%endif

%description
The device-policy-manager package provides a daemon which is responsible for
managing device policies.

%files
%manifest device-policy-manager.manifest
%defattr(644,root,root,755)
%attr(755,root,root) %{_bindir}/device-policy-manager
%attr(700,root,root) %{_bindir}/factory-reset
%dir %{TZ_SYS_DATA}/dpm
%dir %{TZ_SYS_ETC}/dpm/policy
%{TZ_SYS_ETC}/dpm/policy/PolicyManifest.xml
%{_unitdir}/device-policy-manager.service
%{_unitdir}/multi-user.target.wants/device-policy-manager.service

%prep
%setup -q

%build
%{!?build_type:%define build_type "RELEASE"}

%if %{build_type} == "DEBUG" || %{build_type} == "PROFILING" || %{build_type} == "CCOV"
	CFLAGS="$CFLAGS -Wp,-U_FORTIFY_SOURCE"
	CXXFLAGS="$CXXFLAGS -Wp,-U_FORTIFY_SOURCE"
%endif

%{!?profile:%define profile "mobile"}

%cmake . -DVERSION=%{version} \
         -DCMAKE_BUILD_TYPE=%{build_type} \
         -DTIZEN_PROFILE_NAME=%{profile} \
         -DSCRIPT_INSTALL_DIR=%{_scriptdir} \
         -DSYSTEMD_UNIT_INSTALL_DIR=%{_unitdir} \
         -DDATA_INSTALL_DIR=%{TZ_SYS_DATA}/dpm \
         -DCONF_INSTALL_DIR=%{TZ_SYS_ETC}/dpm \
         -DDB_INSTALL_DIR=%{TZ_SYS_DB} \
         -DRUN_INSTALL_DIR=%{TZ_SYS_RUN} \
         -DAPP_INSTALL_PREFIX="%{TZ_SYS_RO_APP}" \
         -DAPP_SHARE_PACKAGES_DIR="%{TZ_SYS_RO_PACKAGES}" \
         -DPAMD_INSTALL_DIR=/etc/pam.d

make %{?jobs:-j%jobs}

%install
%make_install
mkdir -p %{buildroot}/%{_unitdir}/multi-user.target.wants
ln -s ../device-policy-manager.service %{buildroot}/%{_unitdir}/multi-user.target.wants/device-policy-manager.service

%clean
rm -rf %{buildroot}

%preun

%postun

## Client Package #############################################################
%package -n libdpm
Summary: Tizen Device Policy Client library
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description -n libdpm
The libdpm package contains the libraries needed to run DPM client.

%post -n libdpm -p /sbin/ldconfig

%postun -n libdpm -p /sbin/ldconfig

%files -n libdpm
%manifest device-policy-manager.manifest
%defattr(644,root,root,755)
%attr(755,root,root) %{_libdir}/libdpm.so.%{version}
%{_libdir}/libdpm.so.0

## Devel Package ##############################################################
%package -n libdpm-devel
Summary: Libraries and header files for device policy client development
Group: Development/Libraries
Requires: libdpm = %{version}-%{release}

%description -n libdpm-devel
The libdpm-devel package includes the libraries and header files necessary for
developing the DPM client program.

%files -n libdpm-devel
%manifest device-policy-manager.manifest
%defattr(644,root,root,755)
%{_libdir}/libdpm.so
%{_includedir}/dpm
%{_libdir}/pkgconfig/dpm.pc

## Test Package ##############################################################
%package -n dpm-testcases
Summary: Device Policy Manager test cases
Group: Development/Libraries
Requires: libdpm = %{version}-%{release}

%description -n dpm-testcases
Testcases for device policy manager and device policy client

%files -n dpm-testcases
%defattr(644,root,root,755)
%attr(755,root,root) %{_bindir}/dpm-unit-tests
%attr(755,root,root) %{_bindir}/dpm-integration-tests
%attr(755,root,root) %{_bindir}/dpm-api-tests
%defattr(-,root,root,-)
%{TZ_SYS_DATA}/dpm/sample-policy.xml

## Tools Package #############################################################
%package -n org.tizen.ode
Summary: Tizen ODE User Interface
Group: Security/Other
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(efl-extension)
BuildRequires: pkgconfig(libxml-2.0)
BuildRequires: pkgconfig(capi-system-system-settings)
BuildRequires: pkgconfig(evas)
BuildRequires: pkgconfig(dpm)
BuildRequires: pkgconfig(capi-system-device)
BuildRequires: pkgconfig(capi-system-runtime-info)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(capi-appfw-app-control)

%description -n org.tizen.ode
Tizen ODE User Interface for device policy management

%define odeapp_home %{TZ_SYS_RO_APP}/org.tizen.ode

%files -n org.tizen.ode
%defattr(-,root,root,-)
%{odeapp_home}/bin/*
%{odeapp_home}/res/*
%{TZ_SYS_RO_PACKAGES}/org.tizen.ode.xml

## DPM Syspopup Package ######################################################
%package -n org.tizen.dpm-syspopup
Summary: Tizen DPM system popup Interface
Group: Security/Other
BuildRequires: pkgconfig(efl-extension)
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(capi-system-system-settings)
BuildRequires: pkgconfig(capi-ui-efl-util)

%description -n org.tizen.dpm-syspopup
Tizen DPM system popup interface package

%files -n org.tizen.dpm-syspopup
%defattr(-,root,root,-)
%manifest tools/syspopup/org.tizen.dpm-syspopup.manifest
%{TZ_SYS_RO_APP}/org.tizen.dpm-syspopup/bin/*
%{TZ_SYS_RO_APP}/org.tizen.dpm-syspopup/res/locale/*
%{TZ_SYS_RO_PACKAGES}/org.tizen.dpm-syspopup.xml
/usr/share/icons/default/small/org.tizen.dpm-syspopup.png

## Begin of mobile feature ###################################################
%if "%{profile}" == "mobile"

## ZONE Setup Wizard Package #################################################
%package -n org.tizen.zone-setup-wizard
Summary: Tizen ZONE Setup wizard Interface
Group: Security/Other
BuildRequires: pkgconfig(efl-extension)
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(evas)
BuildRequires: pkgconfig(notification)
Requires: libzone = %{version}-%{release}

%description -n org.tizen.zone-setup-wizard
Tizen ZONE setup wizard interface for zone

%define setup_home %{TZ_SYS_RO_APP}/org.tizen.zone-setup-wizard

%files -n org.tizen.zone-setup-wizard
%defattr(-,root,root,-)
%manifest zone/setup-wizard/org.tizen.zone-setup-wizard.manifest
%{setup_home}/bin/*
%{setup_home}/res/*
%{setup_home}/res/data/ZoneManifest.xml
%{TZ_SYS_RO_PACKAGES}/org.tizen.zone-setup-wizard.xml

## Kaskit package #####################################################
%package -n org.tizen.kaskit
Summary: Tizen Krate launcher Interface
Group: Security/Other
BuildRequires: pkgconfig(efl-extension)
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(evas)

%description -n org.tizen.kaskit
Tizen Krate launcher interface for zone

%define launcher_home %{TZ_SYS_RO_APP}/org.tizen.kaskit

%files -n org.tizen.kaskit
%defattr(-,root,root,-)
%manifest zone/kaskit/org.tizen.kaskit.manifest
%{launcher_home}/bin/*
%{launcher_home}/res/*
%{launcher_home}/res/images/default_app_icon.png
%{TZ_SYS_RO_PACKAGES}/org.tizen.kaskit.xml

## Zone Client Package ########################################################
%package -n libzone
Summary: Tizen Zone Client library
Group: Development/Libraries
BuildRequires: pkgconfig(capi-appfw-package-manager)
BuildRequires: pkgconfig(capi-appfw-app-manager)
BuildRequires: pkgconfig(libtzplatform-config)
Requires: %{name} = %{version}-%{release}
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description -n libzone
The libzone package contains the libraries needed to control inside of the zone.

%post -n libzone -p /sbin/ldconfig

%postun -n libzone -p /sbin/ldconfig

%files -n libzone
%manifest device-policy-manager.manifest
%defattr(644,root,root,755)
%attr(755,root,root) %{_libdir}/libzone.so.%{version}
%{_libdir}/libzone.so.0

## Devel Package ##############################################################
%package -n libzone-devel
Summary: Libraries and header files for zone client development
Group: Development/Libraries
Requires: libzone = %{version}-%{release}

%description -n libzone-devel
The libzone-devel package includes the libraries and header files necessary for
developing the zone client program.

%files -n libzone-devel
%manifest device-policy-manager.manifest
%defattr(644,root,root,755)
%{_libdir}/libzone.so
%{_includedir}/zone
%{_libdir}/pkgconfig/zone.pc

## PAM Plugin Package ########################################################
%package -n dpm-pam-zone
Summary: PAM Plugin for zone policy in device policy manager
Group: Development/Libraries
Requires: systemd

%description -n dpm-pam-zone
PAM Plugin for zone policy in device policy manager and CLI tool

%post -n dpm-pam-zone
mv /etc/pam.d/systemd-user /etc/pam.d/systemd-user.keep
cp /etc/pam.d/systemd-user-zone /etc/pam.d/systemd-user

%postun -n dpm-pam-zone
mv /etc/pam.d/systemd-user.keep /etc/pam.d/systemd-user

%files -n dpm-pam-zone
%manifest device-policy-manager.manifest
%defattr(600,root,root,700)
%attr(700,root,root) %{_libdir}/security/pam_*.so
%attr(700,root,root) %{_sbindir}/zone-admin-cli
%attr(700,root,root) %{_sbindir}/zone-volume-manager
%attr(700,root,root) %dir %{TZ_SYS_ETC}/dpm/zone
%attr(600,root,root) %config %{TZ_SYS_ETC}/dpm/zone/owner.xml
%attr(644,root,root) /opt/data/dpm/zone-indicator.png
%config /etc/pam.d/*

%endif
# End of mobile feature
