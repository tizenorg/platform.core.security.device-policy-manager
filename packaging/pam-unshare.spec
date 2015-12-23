Name: pam-unshare
Version: 0.1
Release: 0
ExclusiveArch:  %arm
Summary: PAM module and CLI tool
Group: Security/Access Control
License: Apache-2.0
Source0: %{name}-%{version}.tar.gz
BuildRequires:  gcc
BuildRequires:  cmake
BuildRequires:  pam-devel

%description
This package contains PAM modules for use for unshareing namespaces and related CLI tools.

%prep
%setup -q

%build
%cmake . -DVERSION=%{version} \
         -DCMAKE_BUILD_TYPE=%{build_type} \
	 -DCMAKE_INSTALL_PREFIX=%{_prefix}

make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}

%make_install

%clean
rm -rf %{buildroot}

%files
%attr(700,root,root) %{_libdir}/security/pam_*.so
%attr(700,root,root) %{_sbindir}/nsattach
%config /etc/pam.d
%config /etc/security
